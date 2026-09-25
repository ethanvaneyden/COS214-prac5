#ifndef LEGACY_ALARM_ADAPTER_H
#define LEGACY_ALARM_ADAPTER_H

#include "AlarmSystem.h"
#include <map>
#include <memory>
#include <string>

class LegacyAlarmSystem;

/**
 * @brief Adapter. Lets CampusGuard drive the existing legacy siren
 *        hardware through the AlarmSystem interface the rest of the
 *        application actually wants to use.
 *
 * Translates a human-readable location into the hardware's integer
 * zone code, and an Incident-style severity level (1-4) into the
 * hardware's integer mode (1-3), then forwards to the adaptee and turns
 * its return-code failures into exceptions so callers cannot silently
 * ignore a rejected alert. Owns the LegacyAlarmSystem instance, since
 * nothing else in CampusGuard has a reason to reach it directly.
 */
class LegacyAlarmAdapter : public AlarmSystem {
private:
  std::unique_ptr<LegacyAlarmSystem> legacySystem;
  std::map<std::string, int> zoneLookup;

  /// @throws std::invalid_argument if @p location has no known hardware
  ///         zone. This is the "unknown location" failure case; it is
  ///         never swallowed.
  int resolveZone(const std::string &location) const;

  /// @brief Map an Incident-style severity level (1-4) to a legacy mode
  ///        (1-3): the legacy hardware has no "critical" tier of its own,
  ///        so High and Critical both map to its most urgent mode.
  int toLegacyMode(int level) const;

public:
  LegacyAlarmAdapter();
  virtual ~LegacyAlarmAdapter() = default;

  void triggerAlert(const std::string &location, int level) override;
  void silenceAlert(const std::string &location) override;

  /// @brief Register (or overwrite) a location -> hardware zone mapping.
  ///        Exposed so tests, or a future building-management import,
  ///        can extend the wiring without changing the adapter's code.
  void registerZone(const std::string &location, int zoneCode);
};

#endif
