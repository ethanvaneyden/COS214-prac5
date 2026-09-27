#include "LegacyAlarmAdapter.h"
#include "LegacyAlarmSystem.h"
#include <stdexcept>

using namespace std;

LegacyAlarmAdapter::LegacyAlarmAdapter() : legacySystem(new LegacyAlarmSystem()) {
  // Zones wired into the legacy hardware at install time. In the real
  // system this would be loaded from a building-management config file;
  // hardcoding it here keeps the adapter self-contained for the demo.
  zoneLookup["LibraryEast"] = 1;
  zoneLookup["LibraryWest"] = 2;
  zoneLookup["ScienceBlock"] = 3;
  zoneLookup["MainGate"] = 4;
  zoneLookup["Residence1"] = 5;
  zoneLookup["Residence2"] = 6;
  zoneLookup["SportsHall"] = 7;
  zoneLookup["AdminBlock"] = 8;
}

int LegacyAlarmAdapter::resolveZone(const string &location) const {
  auto it = zoneLookup.find(location);
  if (it == zoneLookup.end()) {
    throw invalid_argument(
        "LegacyAlarmAdapter: no hardware zone wired for location '" +
        location + "'");
  }
  return it->second;
}

int LegacyAlarmAdapter::toLegacyMode(int level) const {
  if (level <= 1) {
    return 1; // Low
  }
  if (level == 2) {
    return 2; // Medium
  }
  return 3; // High / Critical 
}

void LegacyAlarmAdapter::triggerAlert(const string &location, int level) {
  int zone = resolveZone(location);
  int mode = toLegacyMode(level);

  int result = legacySystem->setAlarm(zone, mode);
  if (result != 0) {
    throw runtime_error(
        "LegacyAlarmAdapter: hardware rejected setAlarm for zone " +
        to_string(zone));
  }
}

void LegacyAlarmAdapter::silenceAlert(const string &location) {
  int zone = resolveZone(location);

  int result = legacySystem->clearAlarm(zone);
  if (result != 0) {
    throw runtime_error(
        "LegacyAlarmAdapter: hardware rejected clearAlarm for zone " +
        to_string(zone));
  }
}

void LegacyAlarmAdapter::registerZone(const string &location, int zoneCode) {
  zoneLookup[location] = zoneCode;
}
