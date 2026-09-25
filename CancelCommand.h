#ifndef CANCEL_COMMAND_H
#define CANCEL_COMMAND_H

#include "Command.h"
#include <string>

class Incident;

/**
 * @brief Concrete Command. Represents the operator's "cancel" action.
 *
 * Receiver: Incident. execute() calls Incident::cancel(), legal from
 * ReportedState or DispatchedState. On success this also causes the
 * ControlRoom (Mediator) to be notified via
 * IncidentState::handleCancellation().
 */
class CancelCommand : public Command {
private:
  Incident *incident;

public:
  explicit CancelCommand(Incident *incident);
  virtual ~CancelCommand() = default;

  void execute() override;
  std::string description() const override;
};

#endif
