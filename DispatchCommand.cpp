#include "DispatchCommand.h"
#include "Incident.h"
#include <stdexcept>

using namespace std;

DispatchCommand::DispatchCommand(Incident *incident) : incident(incident) {}

void DispatchCommand::execute() {
  if (!incident) {
    throw invalid_argument("DispatchCommand: no incident supplied");
  }
  incident->escalate();
}

string DispatchCommand::description() const {
  if (!incident) {
    return "Dispatch <no incident>";
  }
  return "Dispatch " + incident->getRequiredUnitType() + " unit for " +
         incident->getId() + " at " + incident->getLocation();
}
