#ifndef REPORTEDSTATE_H
#define REPORTEDSTATE_H
#include "IncidentState.h"

class ReportedState : public IncidentState {

public:
  virtual ~ReportedState() = default;

  ReportedState() = default;

  /// System dispatches the required unit for this incident.
  void handleEscalation(Incident *context);

  /// Operator marks the incident resolved.
  void handleResolution(Incident *context);

  /// Operator cancels the incident.
  void handleCancellation(Incident *context);

  /// Human-readable name used in logs and the CLI listing.
  std::string getStateName() const;
};

#endif