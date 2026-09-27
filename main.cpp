#include <iostream>
#include <limits>
#include "OperationsDesk.h"

using namespace std;

// Static tests
void testControlRoom()
{
}

void testState()
{
}

void testCommand()
{
}

void testFactory()
{
}

void testOperationsDesk()
{
}

void testAdaptor()
{
}

// Interactive methods

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

void pause(const string &note = "") {
    cout << "\n[press Enter";
    if (!note.empty()) cout << ": " << note;
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
        cout << "(units already registered - continuing)\n";
    }
    pause();

    step(2, "Register an incident (Facade + Mediator)");
    cout << "The operator makes ONE call: desk.reportIncident(...).\n"
            "Behind it: the registry creates and stores the incident,\n"
            "then the mediator fans out incidentReported to every unit.\n\n";

    string id = desk.reportIncident("medical", "Science Building",
                                    "Student collapsed in lab", 4);
    cout << "\nReturned id: " << id << "\n";
    pause();

    // -----------------------------------------------------------
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

    // -----------------------------------------------------------
    step(4, "Colleague-initiated cascade (Mediator in action)");
    cout << "Above, Security discovered a gas leak on scene and told\n"
            "the mediator via hazardDetected(). Maintenance fixed it\n"
            "and told the mediator via areaSecured(). Medical waited\n"
            "for that before entering. Security holds NO pointer to\n"
            "Maintenance or Medical. Neither does anyone else.\n"
            "That coordination is the mediator's entire job.\n";
    pause();

    // -----------------------------------------------------------
    step(5, "Resolve the incident (Command + State)");
    cout << "A second command - ResolveCommand - is legal from the\n"
            "Dispatched state. The state transitions to Resolved and\n"
            "notifies the mediator, which tells every unit to stand\n"
            "down.\n\n";

    desk.dispatchAction(id, "resolve");
    pause();

    // -----------------------------------------------------------
    step(6, "Illegal operation, handled cleanly");
    cout << "Trying to cancel an incident that is already resolved.\n"
            "The state throws InvalidStateTransitionException. The\n"
            "facade catches it and prints a rejection message. No\n"
            "crash, no silent failure.\n\n";

    desk.dispatchAction(id, "cancel");
    pause();

    // -----------------------------------------------------------
    step(7, "Multi-step facade workflow (Facade + Adapter)");
    cout << "One call - evacuateBuilding - runs FOUR subsystem\n"
            "operations behind the scenes:\n"
            "  1. Adapter triggers the legacy alarm system\n"
            "  2. Mediator broadcasts an emergency alert to all units\n"
            "  3. Registry creates an evacuation incident\n"
            "  4. Command pipeline dispatches the required unit\n\n";

    desk.evacuateBuilding("Engineering Building");
    pause();

    // -----------------------------------------------------------
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
            desk.reportIncident(type, location, description, severity);
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
            string type = readLine("Unit type (security/medical/maintenance/comms): ");
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
            desk.evacuateBuilding(readLine("Building: "));
            break;
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

int main()
{
    // Static testing
    testState();
    testControlRoom();
    testCommand();
    testFactory();
    testOperationsDesk();
    testAdaptor();

    /*OperationsDesk desk;

    while (true)
    {
        cout << "\n"
                  << "==============================================\n"
                  << "  CampusGuard\n"
                  << "==============================================\n"
                  << "  1. Guided demo (recommended)\n"
                  << "  2. Free mode (manual console)\n"
                  << "  0. Exit\n> ";

        int choice = readInt("");
        if (choice == -1)
        {
            cout << "Invalid input. Exiting.\n";
            return 0;
        }

        switch (choice)
        {
        case 1:
            runGuidedDemo(desk);
            break;
        case 2:
            runFreeMode(desk);
            break;
        case 0:
            cout << "Goodbye.\n";
            return 0;
        default:
            cout << "Unknown option.\n";
        }
    }*/
}