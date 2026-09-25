#ifndef DISPATCHEDSTATE_H
#define DISPATCHEDSTATE_H
#include "IncidentState.h"

class DispatchedState : public IncidentState
{
  /**
   * @brief Represents an incidents where teams are dispatched but not yet marked as resolved by the operator
   *
   */
public:
  virtual ~DispatchedState() = default;

  DispatchedState() = default;

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