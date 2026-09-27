#include "ResolveCommand.h"
#include "Incident.h"
#include <stdexcept>

using namespace std;

ResolveCommand::ResolveCommand(Incident *incident) : incident(incident) {}

void ResolveCommand::execute() {
  if (!incident) {
    throw invalid_argument("ResolveCommand: no incident supplied");
  }
  incident->resolve();
}

string ResolveCommand::description() const {
  if (!incident) {
    return "Resolve <no incident>";
  }
  return "Resolve " + incident->getId() + " at " + incident->getLocation();
}
