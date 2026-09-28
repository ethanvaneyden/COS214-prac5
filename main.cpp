#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <utility>

#include "OperationsDesk.h"
#include "ControlRoom.h"
#include "Incident.h"
#include "CommandInvoker.h"
#include "DispatchCommand.h"
#include "ResolveCommand.h"
#include "CancelCommand.h"
#include "ResponseUnit.h"
#include "ResponseUnitFactory.h"
#include "SecurityUnitFactory.h"
#include "MedicalUnitFactory.h"
#include "MaintenanceUnitFactory.h"
#include "CommunicationsUnitFactory.h"
#include "LegacyAlarmAdapter.h"
#include "LegacyAlarmSystem.h"
#include "IncidentRegistry.h"

using namespace std;

template <typename Operation>
bool throwsException(Operation operation)
{
    try
    {
        operation();
    }
    catch (const exception &)
    {
        return true;
    }

    return false;
}

class NullUnitFactory : public ResponseUnitFactory
{
public:
    string factoryType() const override
    {
        return "null";
    }

    unique_ptr<ResponseUnit> createUnit(const string &) const override
    {
        return nullptr;
    }
};

void testRegistry();

// -----------------------------------------------------------
// Small test helper
// -----------------------------------------------------------
void testResult(const string &name, bool passed)
{
    cout << "  [" << (passed ? "PASS" : "FAIL") << "] " << name << "\n";
}

// -----------------------------------------------------------
// Static tests
// -----------------------------------------------------------
void testState()
{
    cout << "\n=== Static Test: State ===\n";

    try
    {
        Incident incident("TEST-STATE", "Test Location", "State test",
                          "medical", Incident::Severity::High, nullptr);

        testResult("Initial state is Reported",
                   incident.getStateName() == "Reported");
        testResult("Incident accessors preserve supplied data",
                   incident.getId() == "TEST-STATE" &&
                       incident.getLocation() == "Test Location" &&
                       incident.getDescription() == "State test" &&
                       incident.getType() == "medical" &&
                       incident.getSeverity() == Incident::Severity::High &&
                       incident.getSeverityNumber() == 3 &&
                       incident.getRequiredUnitType() == "Medical" &&
                       incident.getControlRoom() == nullptr);
        testResult("Unmapped incident type defaults to Security",
                   Incident("TEST-FALLBACK", "Somewhere", "Unknown type",
                            "unknown", Incident::Severity::Low, nullptr)
                           .getRequiredUnitType() == "Security");
        testResult("Reported incident cannot be resolved",
                   throwsException([&incident]() { incident.resolve(); }));

        incident.escalate();
        testResult("Reported -> Dispatched",
                   incident.getStateName() == "Dispatched");

        incident.resolve();
        testResult("Dispatched -> Resolved",
                   incident.getStateName() == "Resolved");

        bool rejected = false;
        try
        {
            incident.cancel();
        }
        catch (const exception &)
        {
            rejected = true;
        }

        testResult("Illegal transition from Resolved is rejected", rejected);

        testResult("Resolved incident cannot be escalated or resolved again",
                   throwsException([&incident]() { incident.escalate(); }) &&
                       throwsException([&incident]() { incident.resolve(); }));

        Incident cancelled("TEST-CANCEL", "Test Location", "Cancel test",
                           "theft", Incident::Severity::Low, nullptr);
        cancelled.cancel();
        testResult("Reported -> Cancelled",
                   cancelled.getStateName() == "Cancelled");
        testResult("Cancelled incident rejects every further transition",
                   throwsException([&cancelled]() { cancelled.escalate(); }) &&
                       throwsException([&cancelled]() { cancelled.resolve(); }) &&
                       throwsException([&cancelled]() { cancelled.cancel(); }));

        Incident dispatched("TEST-DISPATCHED", "Test Location", "Cancel after dispatch",
                            "facility", Incident::Severity::Medium, nullptr);
        dispatched.escalate();
        testResult("Dispatched incident cannot be escalated twice",
                   throwsException([&dispatched]() { dispatched.escalate(); }));
        dispatched.cancel();
        testResult("Dispatched -> Cancelled",
                   dispatched.getStateName() == "Cancelled");
        dispatched.setState(nullptr);
        testResult("Missing state reports Unknown and rejects transitions",
                   dispatched.getStateName() == "Unknown" &&
                       throwsException([&dispatched]() { dispatched.escalate(); }) &&
                       throwsException([&dispatched]() { dispatched.resolve(); }) &&
                       throwsException([&dispatched]() { dispatched.cancel(); }));
    }
    catch (const exception &e)
    {
        cout << "  [FAIL] State test threw: " << e.what() << "\n";
    }
}

void testControlRoom()
{
    cout << "\n=== Static Test: Mediator / ControlRoom ===\n";

    try
    {
        LegacyAlarmAdapter alarm;
        ControlRoom room;

        room.registerFactory(
            "security",
            unique_ptr<ResponseUnitFactory>(new SecurityUnitFactory()));
        room.registerFactory(
            "medical",
            unique_ptr<ResponseUnitFactory>(new MedicalUnitFactory()));
        room.registerFactory(
            "maintenance",
            unique_ptr<ResponseUnitFactory>(new MaintenanceUnitFactory()));
        room.registerFactory(
            "comms",
            unique_ptr<ResponseUnitFactory>(new CommunicationsUnitFactory(&alarm)));

        room.createUnit("security", "SEC-T1");
        room.createUnit("medical", "MED-T1");
        room.createUnit("maintenance", "MNT-T1");
        room.createUnit("comms", "COM-T1");

        testResult("Security unit registered",
                   room.findUnit("SEC-T1") != nullptr);
        testResult("Medical unit registered",
                   room.findUnit("MED-T1") != nullptr);
        testResult("Maintenance unit registered",
                   room.findUnit("MNT-T1") != nullptr);
        testResult("Communications unit registered",
                   room.findUnit("COM-T1") != nullptr);
        testResult("Unknown unit lookup returns null",
               room.findUnit("MISSING") == nullptr);

        room.registerFactory("null", unique_ptr<ResponseUnitFactory>(new NullUnitFactory()));
        room.createUnit("missing", "NO-FACTORY");
        room.createUnit("null", "NO-UNIT");

        room.incidentReported("MED-INC", "Science Building", 4);
        room.incidentDispatched("MED-INC", "Science Building", "Medical", 4);
        room.incidentResolved("MED-INC", "Medical");
        room.incidentCancelled("CANCEL-INC");
        room.hazardDetected("Science Building", "gas");
        room.hazardDetected("Science Building", "smoke");
        room.entryRequested("Science Building", "MED-T1");
        room.areaSecured("Science Building");

        room.emergencyAlert("Static mediator test");
        testResult("Mediator emergency broadcast completed", true);

        ControlRoom emptyRoom;
        emptyRoom.listUnits();
        emptyRoom.incidentReported("EMPTY", "Nowhere", 1);
        emptyRoom.incidentDispatched("EMPTY", "Nowhere", "Security", 1);
        emptyRoom.incidentResolved("EMPTY", "Security");
        emptyRoom.incidentCancelled("EMPTY");
        emptyRoom.hazardDetected("Nowhere", "smoke");
        emptyRoom.entryRequested("Nowhere", "Nobody");
        emptyRoom.areaSecured("Nowhere");
        emptyRoom.emergencyAlert("No units registered");
    }
    catch (const exception &e)
    {
        cout << "  [FAIL] ControlRoom test threw: " << e.what() << "\n";
    }
}

void testCommand()
{
    cout << "\n=== Static Test: Command ===\n";

    try
    {
        Incident incident("TEST-CMD", "Test Location", "Command test",
                          "medical", Incident::Severity::Medium, nullptr);
        CommandInvoker invoker;

        invoker.executeCommand(
            unique_ptr<Command>(new DispatchCommand(&incident)));

        testResult("DispatchCommand changes state to Dispatched",
                   incident.getStateName() == "Dispatched");
        testResult("Invoker stores executed dispatch command",
                   invoker.historySize() == 1);

        invoker.executeCommand(
            unique_ptr<Command>(new ResolveCommand(&incident)));

        testResult("ResolveCommand changes state to Resolved",
                   incident.getStateName() == "Resolved");
        testResult("Invoker history contains two commands",
                   invoker.historySize() == 2);

        invoker.printHistory();
        invoker.executeCommand(nullptr);
        testResult("Invoker ignores an empty command", invoker.historySize() == 2);

        CommandInvoker emptyInvoker;
        emptyInvoker.printHistory();
        testResult("Null-incident commands describe and reject cleanly",
                   DispatchCommand(nullptr).description() == "Dispatch <no incident>" &&
                       throwsException([]() { DispatchCommand(nullptr).execute(); }) &&
                       ResolveCommand(nullptr).description() == "Resolve <no incident>" &&
                       throwsException([]() { ResolveCommand(nullptr).execute(); }) &&
                       CancelCommand(nullptr).description() == "Cancel <no incident>" &&
                       throwsException([]() { CancelCommand(nullptr).execute(); }));

        Incident invalidCommand("TEST-INVALID-CMD", "Test Location", "Invalid command",
                                "medical", Incident::Severity::Low, nullptr);
        CommandInvoker failedInvoker;
        bool failedCommandRejected = throwsException([&failedInvoker, &invalidCommand]() {
            failedInvoker.executeCommand(unique_ptr<Command>(new ResolveCommand(&invalidCommand)));
        });
        testResult("Failed command is rejected and not added to history",
                   failedCommandRejected && failedInvoker.historySize() == 0 &&
                       invalidCommand.getStateName() == "Reported");
    }
    catch (const exception &e)
    {
        cout << "  [FAIL] Command test threw: " << e.what() << "\n";
    }
}

void testFactory()
{
    cout << "\n=== Static Test: Factory Method ===\n";

    try
    {
        LegacyAlarmAdapter alarm;

        SecurityUnitFactory securityFactory;
        MedicalUnitFactory medicalFactory;
        MaintenanceUnitFactory maintenanceFactory;
        CommunicationsUnitFactory communicationsFactory(&alarm);

        unique_ptr<ResponseUnit> security = securityFactory.createUnit("SEC-F1");
        unique_ptr<ResponseUnit> medical = medicalFactory.createUnit("MED-F1");
        unique_ptr<ResponseUnit> maintenance = maintenanceFactory.createUnit("MNT-F1");
        unique_ptr<ResponseUnit> comms = communicationsFactory.createUnit("COM-F1");

        testResult("Security factory creates security unit",
                   security && security->getUnitType() == "security");
        testResult("Medical factory creates medical unit",
                   medical && medical->getUnitType() == "medical");
        testResult("Maintenance factory creates maintenance unit",
                   maintenance && maintenance->getUnitType() == "maintenance");
        testResult("Communications factory creates comms unit",
                   comms && comms->getUnitType() == "comms");

        testResult("Factory preserves unit id",
                   security && security->getId() == "SEC-F1");
        testResult("Factories report their registered unit types",
                   securityFactory.factoryType() == "security" &&
                       medicalFactory.factoryType() == "medical" &&
                       maintenanceFactory.factoryType() == "maintenance" &&
                       communicationsFactory.factoryType() == "comms");
    }
    catch (const exception &e)
    {
        cout << "  [FAIL] Factory test threw: " << e.what() << "\n";
    }
}

void testOperationsDesk()
{
    cout << "\n=== Static Test: Facade / OperationsDesk ===\n";

    try
    {
        OperationsDesk desk;

        string invalid = desk.reportIncident(
            "medical", "Test Location", "Invalid severity test", 0);
        testResult("Facade rejects invalid severity", invalid.empty());

        string id = desk.reportIncident(
            "medical", "Test Location", "Facade smoke test", 2);
        testResult("Facade registers a valid incident", !id.empty());

        if (!id.empty())
        {
            desk.dispatchAction(id, "dispatch");
            desk.dispatchAction(id, "resolve");
            testResult("Facade command workflow completes", true);
        }

        desk.dispatchAction("missing-id", "dispatch");
        desk.dispatchAction(id, "unknown-action");

        string cancelId = desk.reportIncident(
            "theft", "Test Location", "Cancel workflow", 1);
        desk.dispatchAction(cancelId, "cancel");
        testResult("Facade cancels a reported incident", true);

        desk.registerUnit("unknown", "UNKNOWN-UNIT");
        desk.broadcastAlert("Facade broadcast");
        desk.listIncidents();
        desk.listUnits();

        desk.evacuateBuilding("Science Building");
        testResult("Facade completes evacuation at a wired building", true);
        testResult("Facade propagates adapter errors for unwired buildings",
                   throwsException([&desk]() { desk.evacuateBuilding("Unwired Building"); }));
    }
    catch (const exception &e)
    {
        cout << "  [FAIL] OperationsDesk test threw: " << e.what() << "\n";
    }
}

void testAdaptor()
{
    cout << "\n=== Static Test: Adapter ===\n";

    try
    {
        LegacyAlarmAdapter adapter;
        adapter.registerZone("TEST-ZONE", 8);

        adapter.triggerAlert("TEST-ZONE", 4);
        adapter.triggerAlert("TEST-ZONE", 1);
        adapter.triggerAlert("TEST-ZONE", 2);
        adapter.triggerAlert("TEST-ZONE", 3);
        adapter.silenceAlert("TEST-ZONE");
        testResult("Adapter translates trigger/silence calls", true);

        bool rejected = false;
        try
        {
            adapter.triggerAlert("UNKNOWN-ZONE", 2);
        }
        catch (const exception &)
        {
            rejected = true;
        }

        testResult("Adapter rejects unknown location", rejected);

        testResult("Adapter rejects out-of-range alert levels",
                   throwsException([&adapter]() { adapter.triggerAlert("TEST-ZONE", 0); }) &&
                       throwsException([&adapter]() { adapter.triggerAlert("TEST-ZONE", 5); }));
        testResult("Adapter rejects invalid hardware zones",
                   throwsException([&adapter]() { adapter.registerZone("BAD-ZONE", 0); }) &&
                       throwsException([&adapter]() { adapter.registerZone("BAD-ZONE", 9); }));

        LegacyAlarmSystem legacy;
        testResult("Legacy alarm reports default, set, and cleared status",
                   legacy.getZoneStatus(3) == "MODE:0" &&
                       legacy.setAlarm(3, 2) == 0 &&
                       legacy.getZoneStatus(3) == "MODE:2" &&
                       legacy.clearAlarm(3) == 0 &&
                       legacy.getZoneStatus(3) == "MODE:0");
        testResult("Legacy alarm rejects unsupported zones",
                   legacy.setAlarm(0, 1) == -1 && legacy.setAlarm(9, 1) == -1 &&
                       legacy.clearAlarm(9) == -1);
    }
    catch (const exception &e)
    {
        cout << "  [FAIL] Adapter test threw: " << e.what() << "\n";
    }
}

void runStaticTests()
{
    cout << "\n==============================================\n"
         << "  CampusGuard - Static Tests\n"
         << "==============================================\n";

    testState();
    testControlRoom();
    testCommand();
    testFactory();
    testOperationsDesk();
    testAdaptor();
    testRegistry();

    cout << "\n=== Static tests complete ===\n";
}

void testRegistry()
{
    cout << "\n=== Static Test: Incident Registry ===\n";

    IncidentRegistry registry;
    testResult("Empty registry lookup returns null",
               registry.find("inc-001") == nullptr);
    registry.printIncidents();
    testResult("Registry rejects severity below and above range",
               registry.createIncident("Nowhere", "Bad low", "medical", 0, nullptr) == nullptr &&
                   registry.createIncident("Nowhere", "Bad high", "medical", 5, nullptr) == nullptr);

    Incident *first = registry.createIncident("Library", "Smoke", "fire", 4, nullptr);
    Incident *second = registry.createIncident("Clinic", "Injury", "medical", 2, nullptr);
    testResult("Registry creates sequential incidents and finds them",
               first != nullptr && second != nullptr &&
                   first->getId() == "inc-001" && second->getId() == "inc-002" &&
                   registry.find("inc-001") == first && registry.find("unknown") == nullptr);
    registry.printIncidents();
}

// -----------------------------------------------------------
// Interactive methods
// -----------------------------------------------------------
void printMenu()
{
    cout << "\n=== CampusGuard ===\n"
         << "  1. Report incident\n"
         << "  2. Dispatch unit to incident\n"
         << "  3. Resolve incident\n"
         << "  4. Cancel incident\n"
         << "  5. Add response unit\n"
         << "  6. Broadcast emergency alert\n"
         << "  7. Evacuate building\n"
         << "  8. List incidents\n"
         << "  9. List units\n"
         << "  0. Exit\n> ";
}

int readInt(const string &prompt)
{
    cout << prompt;
    int value;

    if (!(cin >> value))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

string readLine(const string &prompt)
{
    cout << prompt;
    string line;
    getline(cin, line);
    return line;
}

void pause(const string &note = "")
{
    cout << "\n[press Enter";
    if (!note.empty())
        cout << ": " << note;
    cout << "]\n";

    string temp;
    getline(cin, temp);
}

void step(int n, const string &title)
{
    cout << "\n--- Step " << n << ": " << title << " ---\n";
}

void runGuidedDemo(OperationsDesk &desk)
{
    cout << "\n"
         << "==============================================\n"
         << "  CampusGuard - Guided Demo\n"
         << "==============================================\n"
         << "Six design patterns collaborating in one flow:\n"
         << "  Facade, Command, State, Mediator,\n"
         << "  Factory Method, Adapter\n";

    pause("start");

    step(1, "Bootstrap response units (Factory Method)");
    cout << "The facade asks the ControlRoom to create four units.\n"
            "The ControlRoom looks up each unit's factory and calls\n"
            "createUnit(id) polymorphically. No concrete unit class\n"
            "is ever named outside its own factory.\n\n";

    try
    {
        desk.registerUnit("security", "SEC-01");
        desk.registerUnit("medical", "MED-01");
        desk.registerUnit("maintenance", "MNT-01");
        desk.registerUnit("comms", "COM-01");
    }
    catch (const exception &e)
    {
        cout << "[Demo] Unit registration warning: " << e.what() << "\n";
    }
    pause();

    step(2, "Register an incident (Facade + Mediator)");
    cout << "The operator makes ONE call: desk.reportIncident(...).\n"
            "Behind it: the registry creates and stores the incident,\n"
            "then the mediator fans out incidentReported to every unit.\n\n";

    string id = desk.reportIncident("medical", "Science Building",
                                    "Student collapsed in lab", 4);

    if (id.empty())
    {
        cout << "[Demo] Incident registration failed. Demo stopped.\n";
        return;
    }

    cout << "\nReturned id: " << id << "\n";
    pause();

    step(3, "Dispatch a unit (Command -> State -> Mediator)");
    cout << "Watch the pattern hand-off:\n"
            "  OperationsDesk builds a DispatchCommand\n"
            "    -> CommandInvoker logs and executes it\n"
            "      -> Command calls Incident::escalate()\n"
            "        -> ReportedState decides the transition\n"
            "          -> State notifies the Mediator\n"
            "            -> Mediator fans out to every colleague\n\n";

    desk.dispatchAction(id, "dispatch");
    pause();

    step(4, "Colleague-initiated cascade (Mediator in action)");
    cout << "Above, Security discovered a gas leak on scene and told\n"
        "the mediator via hazardDetected(). Maintenance handled the\n"
        "hazard. Medical then requested safe entry through the mediator,\n"
        "Security secured the area and reported areaSecured(), and only\n"
        "then did Medical enter. Security holds NO pointer to Maintenance\n"
        "or Medical. Coordination happens through the ControlRoom mediator.\n";
    pause();

    step(5, "Resolve the incident (Command + State)");
    cout << "A second command - ResolveCommand - is legal from the\n"
            "Dispatched state. The state transitions to Resolved and\n"
            "notifies the mediator, which tells every unit to stand\n"
            "down.\n\n";

    desk.dispatchAction(id, "resolve");
    pause();

    step(6, "Illegal operation, handled cleanly");
    cout << "Trying to cancel an incident that is already resolved.\n"
            "The state throws InvalidStateTransitionException. The\n"
            "facade catches it and prints a rejection message. No\n"
            "crash and no silent failure.\n\n";

    desk.dispatchAction(id, "cancel");
    pause();

    step(7, "Multi-step facade workflow (Facade + Adapter)");
    cout << "One call - evacuateBuilding - runs FOUR subsystem\n"
            "operations behind the scenes:\n"
            "  1. Adapter triggers the legacy alarm system\n"
            "  2. Mediator broadcasts an emergency alert to all units\n"
            "  3. Registry creates an evacuation incident\n"
            "  4. Command pipeline dispatches the required unit\n\n";

    try
    {
        desk.evacuateBuilding("Engineering Building");
    }
    catch (const exception &e)
    {
        cout << "[Demo] Evacuation failed: " << e.what() << "\n";
    }
    pause();

    step(8, "Final state");
    cout << "Incidents registered:\n\n";
    desk.listIncidents();
    cout << "\nUnits registered:\n\n";
    desk.listUnits();
    pause();

    cout << "\n"
         << "==============================================\n"
         << "  Guided demo complete\n"
         << "==============================================\n"
         << "Patterns demonstrated in this run:\n"
         << "  Facade, Command, State, Mediator,\n"
         << "  Factory Method, Adapter\n";
}

void runFreeMode(OperationsDesk &desk)
{
    cout << "\nTip: units must be registered before you can dispatch.\n"
         << "Use option 5, or run the guided demo once.\n";

    while (true)
    {
        printMenu();
        int choice = readInt("");

        if (choice == -1)
        {
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            string type = readLine("Type: ");
            string location = readLine("Location: ");
            string description = readLine("Description: ");
            int severity = readInt("Severity (1-4): ");

            if (severity < 1 || severity > 4)
            {
                cout << "Severity must be from 1 to 4.\n";
                break;
            }

            string id = desk.reportIncident(type, location, description, severity);
            if (!id.empty())
                cout << "Registered incident: " << id << "\n";
            break;
        }

        case 2:
            desk.dispatchAction(readLine("Incident id: "), "dispatch");
            break;

        case 3:
            desk.dispatchAction(readLine("Incident id: "), "resolve");
            break;

        case 4:
            desk.dispatchAction(readLine("Incident id: "), "cancel");
            break;

        case 5:
        {
            string type = readLine(
                "Unit type (security/medical/maintenance/comms): ");
            string id = readLine("Unit id: ");

            try
            {
                desk.registerUnit(type, id);
            }
            catch (const exception &e)
            {
                cout << "[Desk] " << e.what() << "\n";
            }
            break;
        }

        case 6:
            desk.broadcastAlert(readLine("Message: "));
            break;

        case 7:
        {
            try
            {
                desk.evacuateBuilding(readLine("Building: "));
            }
            catch (const exception &e)
            {
                cout << "[Desk] Evacuation failed: " << e.what() << "\n";
            }
            break;
        }

        case 8:
            desk.listIncidents();
            break;

        case 9:
            desk.listUnits();
            break;

        case 0:
            return;

        default:
            cout << "Unknown option.\n";
        }
    }
}

int main(int argc, char *argv[])
{
    // Useful for coverage/static testing without entering the interactive menu.
    if (argc > 1 && string(argv[1]) == "--tests")
    {
        runStaticTests();
        return 0;
    }

    OperationsDesk desk;

    // Useful when you want the assessed story immediately.
    if (argc > 1 && string(argv[1]) == "--demo")
    {
        runGuidedDemo(desk);
        return 0;
    }

    while (true)
    {
        cout << "\n"
             << "==============================================\n"
             << "  CampusGuard\n"
             << "==============================================\n"
             << "  1. Guided demo (recommended)\n"
             << "  2. Free mode (manual console)\n"
             << "  3. Run static tests\n"
             << "  0. Exit\n> ";

        int choice = readInt("");
        if (choice == -1)
        {
            cout << "Invalid input. Try again.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            runGuidedDemo(desk);
            break;

        case 2:
            runFreeMode(desk);
            break;

        case 3:
            runStaticTests();
            break;

        case 0:
            cout << "Goodbye.\n";
            return 0;

        default:
            cout << "Unknown option.\n";
        }
    }
}
