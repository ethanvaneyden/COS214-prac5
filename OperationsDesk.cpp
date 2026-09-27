#include "OperationsDesk.h"
#include "DispatchCommand.h"
#include "CancelCommand.h"
#include "ResolveCommand.h"
#include "InvalidStateTransitionException.h"
#include <iostream>

using namespace std;

OperationsDesk::OperationsDesk()
{
    controlRoom = unique_ptr<ControlRoom>(new ControlRoom());
    registry = unique_ptr<IncidentRegistry>(new IncidentRegistry());
    invoker = unique_ptr<CommandInvoker>(new CommandInvoker());
    alarm = unique_ptr<AlarmSystem>(new LegacyAlarmAdapter());

    //Todo add factory registration
}

string OperationsDesk::reportIncident(const string &type, const string &location, const string &description, int severity)
{
    Incident *newIncident = registry->createIncident(location, description, type, severity, controlRoom.get());

    if(!newIncident) {
        cout << "[Desk] Incident not registered (invalid severity)!\n";
        return "";
    }

    controlRoom->incidentReported(newIncident->getId(), newIncident->getLocation(), newIncident->getSeverityNumber());
    return newIncident->getId();
}

void OperationsDesk::dispatchAction(const string &incidentId, const string &action)
{
    Incident *inc = registry->find(incidentId);
    if (!inc)
    {
        cout << "[Desk] Unknown incident: " << incidentId << "\n";
        return;
    }

    unique_ptr<Command> command;
    if (action == "dispatch")
    {
        command = unique_ptr<Command>(new DispatchCommand(inc));
    }
    else if (action == "resolve")
    {
        command = unique_ptr<Command>(new ResolveCommand(inc));
    }
    else if (action == "cancel")
    {
        command = unique_ptr<Command>(new CancelCommand(inc));
    }
    else
    {
        cout << "[Desk] Invalid command!\n";
        return;
    }

    try
    {
        invoker->executeCommand(move(command));
    }
    catch (exception &e)
    {
        cout << "[Desk] Operation rejected: " << e.what() << "\n";
    }
}

void OperationsDesk::evacuateBuilding(const std::string &buildingId)
{
    alarm->triggerAlert(buildingId, 4);

    controlRoom->emergencyAlert("Evacuate building: " + buildingId);

    string id = reportIncident("evacuation", buildingId, "Building evacuation", 4);

    if (id.empty())
    {
        cout << "[Desk] Evacuation aborted: incident not registered\n";
        return;
    }

    dispatchAction(id, "dispatch");

    cout << "[Desk] Evacuation workflow complete for " << buildingId << "\n";
}

void OperationsDesk::registerUnit(const std::string &type, const std::string &id)
{
    controlRoom->createUnit(type, id);
}

void OperationsDesk::broadcastAlert(const std::string &message)
{
    controlRoom->emergencyAlert(message);
}

void OperationsDesk::listIncidents() const
{
    registry->printIncidents();
}

void OperationsDesk::listUnits() const
{
    controlRoom->listUnits();
}
