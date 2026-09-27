#ifndef COMMUNICATIONSUNIT_H
#define COMMUNICATIONSUNIT_H

#include "ResponseUnit.h"

#include <string>
#include <unordered_map>

class AlarmSystem;

class CommunicationsUnit : public ResponseUnit
{
private:
    AlarmSystem* alarmSystem;
    std::unordered_map<std::string, std::string> activeAlerts;

public:
    CommunicationsUnit(const std::string& id, AlarmSystem* alarmSystem);

    std::string getUnitType() const override;

    void onIncidentReported(const std::string& incidentId, const std::string& location,int severity) override;

    void onIncidentDispatched(const std::string& incidentId, const std::string& location, const std::string& unitType, int severity) override;

    void onIncidentResolved(const std::string& incidentId, const std::string& unitType) override;

    void onIncidentCancelled(const std::string& incidentId) override;

    void onAreaSecured(const std::string& location) override;

    void onEmergencyAlert(const std::string& message) override;
};

#endif
