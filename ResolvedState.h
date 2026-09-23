#ifndef RESOLVEDSTATE_H
#define RESOLVEDSTATE_H
#include "IncidentState.h"

/**
 * @brief Represents a resolved incident state
 * 
 */

class ResolvedState : public IncidentState {

public:
  virtual ~ResolvedState() = default;

  ResolvedState() = default;

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