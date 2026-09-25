#ifndef LEGACY_ALARM_SYSTEM_H
#define LEGACY_ALARM_SYSTEM_H

#include <map>
#include <string>

/**
 * @brief Adaptee. Stand-in for the campus's existing, externally-managed
 *        siren controller.
 *
 * This class simulates hardware whose interface predates CampusGuard and
 * cannot be changed: it addresses sirens by integer hardware zone code
 * and integer mode, and reports success or failure through a return
 * code rather than an exception. It knows nothing about locations,
 * incidents, or severity levels — that incompatibility is exactly what
 * LegacyAlarmAdapter exists to hide.
 */
class LegacyAlarmSystem {
private:
  // zoneCode -> current mode, simulating on-device state for the 8
  // hardware zones physically wired at install time.
  std::map<int, int> zoneModes;

  static const int MIN_ZONE = 1;
  static const int MAX_ZONE = 8;

public:
  LegacyAlarmSystem() = default;
  ~LegacyAlarmSystem() = default;

  /// @brief Arm hardware zone @p zoneCode at @p mode.
  /// @param mode 0 = off, 1 = low alert, 2 = high alert, 3 = evacuate.
  /// @return 0 on success, -1 if @p zoneCode is not a known hardware zone.
  int setAlarm(int zoneCode, int mode);

  /// @brief Disarm hardware zone @p zoneCode (equivalent to mode 0).
  /// @return 0 on success, -1 if @p zoneCode is not a known hardware zone.
  int clearAlarm(int zoneCode);

  /// @brief Raw hardware status string for @p zoneCode, e.g. "MODE:2".
  std::string getZoneStatus(int zoneCode) const;
};

#endif
