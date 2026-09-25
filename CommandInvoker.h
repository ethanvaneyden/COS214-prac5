#ifndef COMMAND_INVOKER_H
#define COMMAND_INVOKER_H

#include <cstddef>
#include <memory>
#include <vector>

class Command;

/**
 * @brief Invoker. OperationsDesk (the Facade) builds a concrete Command
 *        and hands it here; the invoker never inspects which concrete
 *        type it is running.
 *
 * Owns every command it executes, so a receiver (Incident) only needs to
 * outlive the call to execute(), not the invoker's own lifetime. Keeping
 * the executed commands also gives CampusGuard a simple audit trail /
 * undo point without changing how commands are created.
 */
class CommandInvoker {
private:
  std::vector<std::unique_ptr<Command>> history;

public:
  virtual ~CommandInvoker() = default;
  CommandInvoker() = default;

  /// @brief Run @p command immediately, then store it in the history.
  ///        Exceptions thrown by execute() propagate to the caller before
  ///        the command is stored, so a failed action never appears in
  ///        the history as if it had succeeded.
  void executeCommand(std::unique_ptr<Command> command);

  /// @brief Print every executed command's description, in order.
  void printHistory() const;

  /// @brief Number of commands executed so far.
  std::size_t historySize() const;
};

#endif
