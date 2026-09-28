#include "LegacyAlarmAdapter.h"
#include "LegacyAlarmSystem.h"

#include <stdexcept>

using namespace std;

LegacyAlarmAdapter::LegacyAlarmAdapter()
    : legacySystem(new LegacyAlarmSystem())
{
    // Campus buildings wired to legacy alarm hardware zones
    zoneLookup["Science Building"] = 1;
    zoneLookup["Engineering Building"] = 2;
}

int LegacyAlarmAdapter::resolveZone(const string &location) const
{
    auto it = zoneLookup.find(location);

    if (it == zoneLookup.end())
    {
        throw invalid_argument(
            "LegacyAlarmAdapter: no hardware zone wired for location '" +
            location + "'");
    }

    return it->second;
}

int LegacyAlarmAdapter::toLegacyMode(int level) const
{
    if (level < 1 || level > 4)
    {
        throw invalid_argument(
            "LegacyAlarmAdapter: alert level must be between 1 and 4");
    }

    if (level == 1)
    {
        return 1;
    }

    if (level == 2)
    {
        return 2;
    }

    // Severity 3 and 4 both use the legacy system's
    // highest alarm mode.
    return 3;
}

void LegacyAlarmAdapter::triggerAlert(
    const string &location,
    int level)
{
    int zone = resolveZone(location);
    int mode = toLegacyMode(level);

    int result = legacySystem->setAlarm(zone, mode);

    if (result != 0)
    {
        throw runtime_error(
            "LegacyAlarmAdapter: legacy system failed to trigger alarm");
    }
}

void LegacyAlarmAdapter::silenceAlert(
    const string &location)
{
    int zone = resolveZone(location);

    int result = legacySystem->clearAlarm(zone);

    if (result != 0)
    {
        throw runtime_error(
            "LegacyAlarmAdapter: legacy system failed to silence alarm");
    }
}

void LegacyAlarmAdapter::registerZone(
    const string &location,
    int zoneCode)
{
    if (zoneCode < 1 || zoneCode > 8)
    {
        throw invalid_argument(
            "LegacyAlarmAdapter: zone code must be between 1 and 8");
    }

    zoneLookup[location] = zoneCode;
}