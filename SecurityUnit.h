#ifndef SECURITYUNIT_H
#define SECURITYUNIT_H

#include "ResponseUnit.h"

class SecurityUnit : public ResponseUnit
{
public:
    SecurityUnit(const std::string& id);

    std::string getUnitType() const override;

    void onIncidentReported(const std::string& incidentId, const std::string& location, int severity) override;

    void onIncidentDispatched(const std::string& incidentId, const std::string& location, const std::string& unitType, int severity) override;

    void onIncidentResolved(const std::string& incidentId, const std::string& unitType) override;

    void onIncidentCancelled(const std::string& incidentId) override;

    void onEntryRequested(const std::string& location, const std::string& requesterId) override;

    void onAreaSecured(const std::string& location) override;

    void onHazardDetected(const std::string& location, const std::string& hazard) override;
};

#endif
