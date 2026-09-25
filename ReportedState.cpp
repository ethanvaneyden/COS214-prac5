#include "ReportedState.h"
#include "CancelledState.h"
#include "DispatchedState.h"
#include "Incident.h"
#include "IncidentState.h"
#include "InvalidStateTransitionException.h"
#include "ControlRoom.h"
#include <memory>

using namespace std;

void ReportedState::handleEscalation(Incident *incident)
{
  incident->setState(unique_ptr<IncidentState>(new DispatchedState()));

  if (auto *room = incident->getControlRoom())
  {
    room->incidentDispatched(incident->getId(), incident->getLocation(), incident->getRequiredUnitType(), incident->getSeverityNumber());
  }
}

/// Operator marks the incident resolved.
void ReportedState::handleResolution(Incident *incident)
{
  throw InvalidStateTransitionException(
      "Can't resolve an incident that has not been dispatched!");
}

/// Operator cancels the incident.
void ReportedState::handleCancellation(Incident *incident)
{
  incident->setState(unique_ptr<IncidentState>(new CancelledState()));

  if (auto *room = incident->getControlRoom())
  {
    room->incidentCancelled(incident->getId());
  }
}

/// Human-readable name used in logs and the CLI listing.
string ReportedState::getStateName() const { return "Reported"; }