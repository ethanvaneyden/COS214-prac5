#ifndef CONTROLROOM_H
#define CONTROLROOM_H

#include <memory>
#include <string>
#include <vector>
class ResponseUnit;
class ResponseUnitFactory;

/**
 * @brief Mediator. Coordinates ResponseUnits without them knowing about
 *        each other.
 *
 * Owns all registered units and factories. Colleagues hold a non-owning
 * pointer back to the ControlRoom. Routing happens in two ways: state
 * transitions (incidentReported, incidentDispatched, ...) and colleague
 * discoveries (hazardDetected, entryRequested, areaSecured).
 */
class ControlRoom {

private:
  std::vector<std::unique_ptr<ResponseUnit>> registeredUnits;
  std::vector<std::unique_ptr<ResponseUnitFactory>> factories;

public:
  virtual ~ControlRoom() = default;
  ControlRoom() = default;

  /// @brief New incident registered. @param severity 1-4.
  void incidentReported(const std::string &id, const std::string &location,
                        int severity);

  /// @brief Unit dispatched. Cascades to areaSecured() at severity >= 4.
  void incidentDispatched(const std::string &id, const std::string &location,
                          const std::string &unitType, int severity);

  /// @brief Incident resolved by @p unitType.
  void incidentResolved(const std::string &id, const std::string &unitType);

  /// @brief Incident cancelled before resolution.
  void incidentCancelled(const std::string &id);

  /// @brief Area secured; notifies waiting units (e.g. Medical entering).
  void areaSecured(const std::string &areaId);

  /// @brief Free-form broadcast to all units.
  void emergencyAlert(const std::string &message);

  /// @brief Create via factory for @p type and register. @throws std::invalid_argument.
  void createUnit(const std::string &type, const std::string &id);

  /// @brief Non-owning lookup; nullptr if not found.
  ResponseUnit *findUnit(const std::string &unitId) const;

  /// @brief Colleague-reported hazard; routed to units whose domain covers it.
  void hazardDetected(const std::string &location, const std::string &hazard);

  /// @brief Colleague requests area be secured before entering.
  void entryRequested(const std::string &location,
                      const std::string &requesterId);
};

#endif