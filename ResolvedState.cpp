#include "ResolvedState.h"
#include "Incident.h"
#include "IncidentState.h"
#include "InvalidStateTransitionException.h"
#include "ResolvedState.h"

using namespace std;

void ResolvedState::handleEscalation(Incident *incident) {
  throw InvalidStateTransitionException(
      "Can't escalate a resolved incident!");
}

/// Operator marks the incident resolved.
void ResolvedState::handleResolution(Incident *incident) {
 throw InvalidStateTransitionException(
      "Incident is already resolved!");
}

/// Operator cancels the incident.
void ResolvedState::handleCancellation(Incident *incident) {
 throw InvalidStateTransitionException(
      "Can't cancel a resolved incident!");
}

/// Human-readable name used in logs and the CLI listing.
string ResolvedState::getStateName() const { return "Dispatched"; }