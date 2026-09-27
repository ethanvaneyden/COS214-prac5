#include <iostream>
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

void startMenu()
{
    cout << "\n"
         << "==============================================\n"
         << "  CampusGuard\n"
         << "==============================================\n"
         << "  1. Free mode (manual operator console)\n"
         << "  2. Guided demo (full scenario walkthrough)\n"
         << "  0. Exit\n> ";
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
}