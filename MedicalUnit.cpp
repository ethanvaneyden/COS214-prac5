#include "MedicalUnit.h"
#include "ControlRoom.h"

#include <iostream>

MedicalUnit::MedicalUnit(const std::string& id)
    : ResponseUnit(id)
{
}

std::string MedicalUnit::getUnitType() const
{
    return "medical";
}

void MedicalUnit::onIncidentReported(const std::string& incidentId, const std::string& location, int severity)
{
    std::cout << "[Medical " << id << "] Incident " << incidentId << " reported at " << location  << " (severity " << severity << ")\n";
}

void MedicalUnit::onIncidentDispatched(const std::string& incidentId, const std::string& location, const std::string& unitType, int severity)
{
    (void)unitType;
    (void)severity;

    std::cout << "[Medical " << id << "] Mobilising to " << location << " for incident " << incidentId << "\n";
    std::cout << "[Medical " << id << "] Waiting for Security to confirm safe entry\n";

    if (controlRoom != nullptr)
    {
        controlRoom->entryRequested(location, id);
    }
}

void MedicalUnit::onAreaSecured(const std::string& location)
{
    enterArea(location);
}

void MedicalUnit::enterArea(const std::string& location)
{
    std::cout << "[Medical " << id << "] Entering secured area at " << location << "\n";
}

void MedicalUnit::onIncidentResolved(const std::string& incidentId, const std::string& unitType)
{
    (void)unitType;
    std::cout << "[Medical " << id << "] Standing down after incident " << incidentId << " was resolved\n";
}

void MedicalUnit::onIncidentCancelled(const std::string& incidentId)
{
    std::cout << "[Medical " << id << "] Standing down after incident " << incidentId << " was cancelled\n";
}
