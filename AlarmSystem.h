#ifndef ALARM_SYSTEM_H
#define ALARM_SYSTEM_H

#include <string>

/**
 * @brief Adapter target. The interface CampusGuard itself wants to
 *        program against: alerts addressed by human-readable location
 *        and a 1-4 severity level, matching Incident::Severity.
 *
 * OperationsDesk (the Facade) and IssueAlertCommand-style callers only
 * ever see this interface. LegacyAlarmAdapter is currently the only
 * implementation, wrapping the campus's existing siren hardware, but any
 * future alarm vendor can be integrated the same way without touching
 * client code.
 */
class AlarmSystem {
public:
  virtual ~AlarmSystem() = default;

  /// @brief Trigger an alert at @p location.
  /// @param level 1 (Low) - 4 (Critical), matching Incident::Severity.
  virtual void triggerAlert(const std::string &location, int level) = 0;

  /// @brief Silence a previously triggered alert at @p location.
  virtual void silenceAlert(const std::string &location) = 0;
};

#endif
