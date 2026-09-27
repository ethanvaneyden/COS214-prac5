#ifndef COMMUNICATIONSUNIT_H
#define COMMUNICATIONSUNIT_H

#include "ResponseUnit.h"

class AlarmSystem;

class CommunicationsUnit : public ResponseUnit
{
private:
    AlarmSystem* alarmSystem;

public:
    CommunicationsUnit(const std::string& id, AlarmSystem* alarmSystem);

    std::string getUnitType() const override;

    void onIncidentReported(const std::string& incidentId, const std::string& location,int severity) override;

    void onIncidentDispatched(const std::string& incidentId, const std::string& location, const std::string& unitType, int severity) override;

    void onIncidentResolved(const std::string& incidentId, const std::string& unitType) override;

    void onIncidentCancelled(const std::string& incidentId) override;

    void onAreaSecured(const std::string& location) override;
};

#endif
