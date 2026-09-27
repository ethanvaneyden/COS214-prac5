#include "CommandInvoker.h"
#include "Command.h"
#include <iostream>
#include <utility>

using namespace std;

void CommandInvoker::executeCommand(unique_ptr<Command> command) {
  if (!command) {
    return;
  }

  // Let a failed action's exception propagate; only a command that
  // actually succeeded gets recorded in the history.
  command->execute();

  cout << "[CommandInvoker] executed: " << command->description() << endl;
  history.push_back(std::move(command));
}

void CommandInvoker::printHistory() const {
  cout << "--- Command history (" << history.size() << " command(s)) ---"
       << endl;
  for (const auto &cmd : history) {
    cout << "  " << cmd->description() << endl;
  }
}

size_t CommandInvoker::historySize() const { return history.size(); }
