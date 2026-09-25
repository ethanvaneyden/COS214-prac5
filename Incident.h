#ifndef INCIDENT_H
#define INCIDENT_H

#include "IncidentState.h"
#include <map>
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
class Incident
{
public:
  enum class Severity
  {
    Low = 1,
    Medium = 2,
    High = 3,
    Critical = 4
  };

  const std::map<std::string, std::string> unitToRespond = {
      // medical
      {"medical", "Medical"},
      {"assault", "Medical"},
      {"labaccident", "Medical"},

      // security
      {"theft", "Security"},
      {"intruder", "Security"},
      {"evacuation", "Security"},
      {"bombthreat", "Security"},

      // maintenance
      {"facility", "Maintenance"},
      {"fire", "Maintenance"},
      {"gasleak", "Maintenance"},
      {"poweroutage", "Maintenance"},
      {"flooding", "Maintenance"},
  };

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
  const std::string &getId() const;
  const std::string &getLocation() const;
  const std::string &getDescription() const;
  const std::string &getType() const;
  Severity getSeverity() const;
  int getSeverityNumber() const;

  /// @brief Current state's name, for logging and the CLI listing.
  std::string getStateName() const;

  const std::string &getRequiredUnitType() const;
  ControlRoom *getControlRoom() const;

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