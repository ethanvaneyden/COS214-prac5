#include "LegacyAlarmSystem.h"

using namespace std;

int LegacyAlarmSystem::setAlarm(int zoneCode, int mode)
{
    if (zoneCode < MIN_ZONE || zoneCode > MAX_ZONE)
    {
        return -1;
    }

    zoneModes[zoneCode] = mode;
    return 0;
}

int LegacyAlarmSystem::clearAlarm(int zoneCode)
{
    return setAlarm(zoneCode, 0);
}

string LegacyAlarmSystem::getZoneStatus(int zoneCode) const
{
    auto it = zoneModes.find(zoneCode);

    int mode = (it != zoneModes.end())
        ? it->second
        : 0;

    return "MODE:" + to_string(mode);
}