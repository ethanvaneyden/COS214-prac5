#ifndef REPORTEDSTATE_H
#define REPORTEDSTATE_H
#include "IncidentState.h"

/**
 * @brief Represents a reported but still unresolved incident state
 *
 */
class ReportedState : public IncidentState
{

public:
  virtual ~ReportedState() = default;

  ReportedState() = default;

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