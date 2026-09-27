#ifndef OPERATIONS_DESK_H
#define OPERATIONS_DESK_H

#include "ControlRoom.h"
#include "IncidentRegistry.h"
#include "CommandInvoker.h"
#include "AlarmSystem.h"
#include "LegacyAlarmAdapter.h"
#include <memory>
#include <string>

class IncidentRegistry;
class CommandInvoker;
class ControlRoom;
class AlarmSystem;

/**
 * @brief Facade. The operator's single entry point to CampusGuard.
 *
 * Owns the subsystems. Subsystems remain independently usable by
 * tests or other code; the Facade is a convenience, not a barrier.
 *
 * Declaration order matters: controlRoom must be destroyed after
 * registry so Incidents never notify a dead mediator, and the invoker
 * (which holds Commands referencing Incidents) must be destroyed first.
 */
class OperationsDesk
{
public:
  OperationsDesk();
  ~OperationsDesk() = default;

  // --- operator workflows ---

  /// @brief Register a new incident and notify the mediator.
  /// @return The generated incident id (e.g. "INC-001").
  std::string reportIncident(const std::string &type,
                             const std::string &location,
                             const std::string &description, int severity);

  /// @brief Run the given action against an incident.
  /// @param action One of "dispatch", "resolve", "cancel".
  void dispatchAction(const std::string &incidentId, const std::string &action);

  /// @brief Coordinated multi-step evacuation of a building.
  void evacuateBuilding(const std::string &buildingId);

  // --- administrative ---

  /// @brief Create a response unit via the factory for @p type.
  void registerUnit(const std::string &type, const std::string &id);

  /// @brief Broadcast a free-form emergency alert.
  void broadcastAlert(const std::string &message);

  // --- read-only views ---

  void listIncidents() const;
  void listUnits() const;

private:
  // Destruction order = reverse of declaration.
  // invoker destroyed first (Commands reference Incidents).
  // registry destroyed before controlRoom (Incidents observe the mediator).
  std::unique_ptr<ControlRoom> controlRoom;
  std::unique_ptr<IncidentRegistry> registry;
  std::unique_ptr<CommandInvoker> invoker;
  std::unique_ptr<AlarmSystem> alarm;
};

#endif