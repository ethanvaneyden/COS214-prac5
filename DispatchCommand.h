#ifndef DISPATCH_COMMAND_H
#define DISPATCH_COMMAND_H

#include "Command.h"
#include <string>

class Incident;

/**
 * @brief Concrete Command. Represents the operator's "dispatch" action.
 *
 * Receiver: Incident. execute() calls Incident::escalate(), which
 * delegates to the incident's current IncidentState and, on success,
 * notifies the ControlRoom (Mediator). Legal only from ReportedState;
 * an illegal call throws InvalidStateTransitionException, which this
 * command does not catch.
 */
class DispatchCommand : public Command {
private:
  Incident *incident;

public:
  explicit DispatchCommand(Incident *incident);
  virtual ~DispatchCommand() = default;

  void execute() override;
  std::string description() const override;
};

#endif
