#ifndef INCIDENT_H
#define INCIDENT_H

#include "IncidentState.h"
#include <memory>
#include <string>

class ControlRoom;

/**
 * @brief Context for the State pattern and Receiver for Command.
 *
 * The Incident knows which unit type it requires (derived once at
 * construction from its type) and holds a non-owning pointer to the
 * ControlRoom so states can notify the mediator after a transition.
 */
class Incident {
public:
  enum class Severity { Low = 1, Medium = 2, High = 3, Critical = 4 };

  Incident(std::string id, std::string location, std::string description,
           std::string type, Severity severity, ControlRoom *room);

  ~Incident() = default;

  Incident(const Incident &) = delete;
  Incident &operator=(const Incident &) = delete;

  // --- transitions (delegated to the current state) ---

  /// @brief Dispatch the required unit. Legal from Reported / Dispatched.
  void escalate();

  /// @brief Mark resolved. Legal only from Dispatched.
  void resolve();

  /// @brief Cancel before resolution. Legal from Reported / Dispatched.
  void cancel();

  /// @brief Replace the current state. Ownership transfers to the Incident.
  void setState(std::unique_ptr<IncidentState> newState);

  // --- accessors ---
  const std::string &getId() const { return id; }
  const std::string &getLocation() const { return location; }
  const std::string &getDescription() const { return description; }
  const std::string &getType() const { return type; }
  Severity getSeverity() const { return severity; }
  int getSeverityNumber() const {return static_cast<int>(severity);}

  /// @brief Current state's name, for logging and the CLI listing.
  std::string getStateName() const;

  const std::string &getRequiredUnitType() const { return requiredUnitType; }
  ControlRoom *getControlRoom() const { return controlRoom; }

private:
  std::string id;
  std::string location;
  std::string description;
  std::string type;
  Severity severity;
  std::string requiredUnitType;
  std::unique_ptr<IncidentState> state;
  ControlRoom *controlRoom;
};

#endif