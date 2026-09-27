#include "CancelCommand.h"
#include "Incident.h"
#include <stdexcept>

using namespace std;

CancelCommand::CancelCommand(Incident *incident) : incident(incident) {}

void CancelCommand::execute() {
  if (!incident) {
    throw invalid_argument("CancelCommand: no incident supplied");
  }
  incident->cancel();
}

string CancelCommand::description() const {
  if (!incident) {
    return "Cancel <no incident>";
  }
  return "Cancel " + incident->getId() + " at " + incident->getLocation();
}
