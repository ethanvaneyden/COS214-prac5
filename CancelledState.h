#ifndef CANCELLEDSTATE_H
#define CANCELLEDSTATE_H
#include "IncidentState.h"

/**
 * @brief Represents an incident that was cancelled by the operator before being resolved
 *
 */
class CancelledState : public IncidentState
{

public:
  virtual ~CancelledState() = default;

  CancelledState() = default;

  /// System dispatches the required unit for this incident.
  void handleEscalation(Incident *context) override;

  /// Operator marks the incident resolved.
  void handleResolution(Incident *context) override;

  /// Operator cancels the incident.
  void handleCancellation(Incident *context) override;

  /// Human-readable name used in logs and the CLI listing.
  std::string getStateName() const override;
};

#endif