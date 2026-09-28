#include "Incident.h"
#include "IncidentState.h"
#include "InvalidStateTransitionException.h"
#include "ReportedState.h"
#include <memory>

using namespace std;

Incident::Incident(string id, string location, string description, string type, Severity severity, ControlRoom *room)
    : id(id), location(location), description(description), type(type), severity(severity), requiredUnitType(), state(new ReportedState()), controlRoom(room)
{

  auto it = unitToRespond.find(type);
  if (it != unitToRespond.end())
  {
    requiredUnitType = it->second;
  }
  else
  {
    requiredUnitType = "Security";
  }
}

void Incident::escalate()
{
  if (!state)
    throw InvalidStateTransitionException("No state on " + id);
  state->handleEscalation(this);
}

void Incident::resolve()
{
  if (!state)
    throw InvalidStateTransitionException("No state on " + id);
  state->handleResolution(this);
}

void Incident::cancel()
{
  if (!state)
    throw InvalidStateTransitionException("No state on " + id);
  state->handleCancellation(this);
}

void Incident::setState(unique_ptr<IncidentState> newState)
{
  if (!newState)
  {
    throw InvalidStateTransitionException("Incident state cannot be null for " + id);
  }
  state = std::move(newState);
}

const string &Incident::getId() const { return id; }
const string &Incident::getLocation() const { return location; }
const string &Incident::getDescription() const { return description; }
const string &Incident::getType() const { return type; }
Incident::Severity Incident::getSeverity() const { return severity; }
int Incident::getSeverityNumber() const { return static_cast<int>(severity); }

string Incident::getStateName() const
{
  if (!state)
  {
    return "Unknown";
  }
  return state->getStateName();
}

const string &Incident::getRequiredUnitType() const { return requiredUnitType; }
ControlRoom *Incident::getControlRoom() const { return controlRoom; }