#ifndef COMMAND_H
#define COMMAND_H

#include <string>

/**
 * @brief Command interface. Represents a single operator action as an
 *        object, so the CommandInvoker can execute and record actions
 *        without knowing which concrete action it is running.
 *
 * Concrete commands (DispatchCommand, ResolveCommand, CancelCommand, ...)
 * hold a non-owning pointer to their receiver and call one real domain
 * operation on it from execute().
 */
class Command {
public:
  virtual ~Command() = default;

  /// @brief Perform the action against this command's receiver.
  /// @throws InvalidStateTransitionException (or similar) if the receiver
  ///         rejects the action; the command does not swallow errors.
  virtual void execute() = 0;

  /// @brief Human-readable summary, used by CommandInvoker::printHistory()
  ///        and CLI output so a tutor can trace invoker -> receiver.
  virtual std::string description() const = 0;
};

#endif
