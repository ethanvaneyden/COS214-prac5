#ifndef INCIDENT_STATE_H
#define INCIDENT_STATE_H

#include <string>

class Incident;

/**
 * @brief State interface for the Incident lifecycle.
 *
 * Each concrete state decides which transitions are legal. Illegal
 * transitions throw InvalidStateTransitionException.
 */
class IncidentState {
public:
    virtual ~IncidentState() = default;

    /// System dispatches the required unit for this incident.
    virtual void handleEscalation(Incident* context) = 0;

    /// Operator marks the incident resolved.
    virtual void handleResolution(Incident* context) = 0;

    /// Operator cancels the incident.
    virtual void handleCancellation(Incident* context) = 0;

    /// Human-readable name used in logs and the CLI listing.
    virtual std::string getStateName() const = 0;
};

#endif