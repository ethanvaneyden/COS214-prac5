#include "CancelledState.h"
#include "Incident.h"
#include "IncidentState.h"
#include "InvalidStateTransitionException.h"

using namespace std;

void CancelledState::handleEscalation(Incident *incident) {
  throw InvalidStateTransitionException(
      "Can't escalate a cancelled incident!");
}

/// Operator marks the incident resolved.
void CancelledState::handleResolution(Incident *incident) {
throw InvalidStateTransitionException(
      "Can't resolve a cancelled incident!");
}

/// Operator cancels the incident.
void CancelledState::handleCancellation(Incident *incident) {
throw InvalidStateTransitionException(
      "Incident is already cancelled!");
}

/// Human-readable name used in logs and the CLI listing.
string CancelledState::getStateName() const { return "Cancelled"; }