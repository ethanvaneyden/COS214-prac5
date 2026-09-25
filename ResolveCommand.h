#ifndef RESOLVE_COMMAND_H
#define RESOLVE_COMMAND_H

#include "Command.h"
#include <string>

class Incident;

/**
 * @brief Concrete Command. Represents the operator's "resolve" action.
 *
 * Receiver: Incident. execute() calls Incident::resolve(), legal only
 * from DispatchedState. On success this also causes the ControlRoom
 * (Mediator) to be notified via IncidentState::handleResolution().
 */
class ResolveCommand : public Command {
private:
  Incident *incident;

public:
  explicit ResolveCommand(Incident *incident);
  virtual ~ResolveCommand() = default;

  void execute() override;
  std::string description() const override;
};

#endif
