#include "DispatchedState.h"
#include "CancelledState.h"
#include "Incident.h"
#include "IncidentState.h"
#include "InvalidStateTransitionException.h"
#include "ResolvedState.h"
#include "ControlRoom.h"
#include <memory>

using namespace std;

void DispatchedState::handleEscalation(Incident *incident)
{
  throw InvalidStateTransitionException(
      "Can't escalate a dispatched incident!");
}

/// Operator marks the incident resolved.
void DispatchedState::handleResolution(Incident *incident)
{
  incident->setState(unique_ptr<IncidentState>(new ResolvedState()));

  if (auto *room = incident->getControlRoom())
  {
    room->incidentResolved(incident->getId(), incident->getRequiredUnitType());
  }
}

/// Operator cancels the incident.
void DispatchedState::handleCancellation(Incident *incident)
{
  incident->setState(unique_ptr<IncidentState>(new CancelledState()));

  if (auto *room = incident->getControlRoom())
  {
    room->incidentCancelled(incident->getId());
  }
}

/// Human-readable name used in logs and the CLI listing.
string DispatchedState::getStateName() const { return "Dispatched"; }