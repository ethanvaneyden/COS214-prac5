#ifndef MEDICALUNIT_H
#define MEDICALUNIT_H

#include "ResponseUnit.h"

class MedicalUnit : public ResponseUnit
{
public:
    MedicalUnit(const std::string& id);

    std::string getUnitType() const override;

    void onIncidentReported(const std::string& incidentId, const std::string& location, int severity) override;

    void onIncidentDispatched(const std::string& incidentId, const std::string& location, const std::string& unitType, int severity) override;

    void onAreaSecured(const std::string& location) override;

    void enterArea(const std::string& location) override;

    void onIncidentResolved(const std::string& incidentId, const std::string& unitType) override;

    void onIncidentCancelled(const std::string& incidentId) override;
};

#endif
