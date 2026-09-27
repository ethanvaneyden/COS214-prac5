#ifndef MAINTENANCEUNIT_H
#define MAINTENANCEUNIT_H

#include "ResponseUnit.h"

class MaintenanceUnit : public ResponseUnit
{
public:
    MaintenanceUnit(const std::string& id);

    std::string getUnitType() const override;

    void onIncidentReported(const std::string& incidentId, const std::string& location, int severity) override;

    void onIncidentDispatched(const std::string& incidentId, const std::string& location, const std::string& unitType, int severity) override;

    void onHazardDetected(const std::string& location, const std::string& hazard) override;

    void onIncidentResolved(const std::string& incidentId, const std::string& unitType) override;

    void onIncidentCancelled(const std::string& incidentId) override;
};
#endif
