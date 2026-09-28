#include "SecurityUnit.h"
#include "ControlRoom.h"

#include <iostream>

SecurityUnit::SecurityUnit(const std::string& id)
    : ResponseUnit(id)
{
}

std::string SecurityUnit::getUnitType() const
{
    return "security";
}

void SecurityUnit::onIncidentReported(const std::string& incidentId, const std::string& location, int severity)
{
    std::cout << "[Security " << id << "] Incident " << incidentId << " reported at " << location << " (severity " << severity << ")\n";
}

void SecurityUnit::onIncidentDispatched(const std::string& incidentId, const std::string& location, const std::string& unitType,int severity)
{
    (void)unitType;

    std::cout << "[Security " << id << "] Mobilising to " << location << " for incident " << incidentId << "\n";

    if (severity >= 3 && controlRoom != nullptr)
    {
        std::cout << "[Security " << id << "] Hazard detected at " << location << "\n";
        controlRoom->hazardDetected(location, "gas");
    }
}

void SecurityUnit::onIncidentResolved(const std::string& incidentId, const std::string& unitType)
{
    std::cout << "[Security " << id << "] Standing down after incident "  << incidentId << " was resolved by " << unitType << "\n";
}

void SecurityUnit::onIncidentCancelled(const std::string& incidentId)
{
    std::cout << "[Security " << id << "] Standing down after incident " << incidentId << " was cancelled\n";
}

void SecurityUnit::onEntryRequested(const std::string& location, const std::string& requesterId)
{
    std::cout << "[Security " << id << "] Securing " << location << " for " << requesterId << "\n";

    if (controlRoom != nullptr)
    {
        controlRoom->areaSecured(location);
    }
}

void SecurityUnit::onAreaSecured(const std::string& location)
{
    std::cout << "[Security " << id << "] Area confirmed secure at " << location << "\n";
}

void SecurityUnit::onHazardDetected(const std::string& location, const std::string& hazard)
{
    std::cout << "[Security " << id << "] Hazard notification received: " << hazard << " at " << location << "\n";
}
