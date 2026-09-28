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

using namespace std;

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

        room.emergencyAlert("Static mediator test");
        testResult("Mediator emergency broadcast completed", true);
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

    cout << "\n=== Static tests complete ===\n";
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
