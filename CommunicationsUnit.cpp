#include "CommunicationsUnit.h"
#include "AlarmSystem.h"

#include <iostream>

CommunicationsUnit::CommunicationsUnit(const std::string& id, AlarmSystem* alarmSystem)
    : ResponseUnit(id), alarmSystem(alarmSystem)
{
}

std::string CommunicationsUnit::getUnitType() const
{
    return "comms";
}

void CommunicationsUnit::onIncidentReported(const std::string& incidentId, const std::string& location, int severity)
{
    std::cout << "[Communications " << id << "] Incident " << incidentId << " reported at " << location << " (severity " << severity << ")\n";
}

void CommunicationsUnit::onIncidentDispatched(const std::string& incidentId,  const std::string& location, const std::string& unitType, int severity)
{
    (void)unitType;
    if (severity < 3)
    {
        return;
    }

    std::cout << "[Communications " << id << "] Triggering campus alert at " << location << "\n";
    activeAlerts[incidentId] = location;

    if (alarmSystem != nullptr)
    {
        alarmSystem->triggerAlert(location, severity);
    }
}

void CommunicationsUnit::onIncidentResolved(const std::string& incidentId, const std::string& unitType)
{
    (void)unitType;

    const auto it = activeAlerts.find(incidentId);
    if (it == activeAlerts.end())
    {
        return;
    }

    if (alarmSystem != nullptr)
    {
        alarmSystem->silenceAlert(it->second);
    }

    std::cout << "[Communications " << id << "] Alert silenced at " << it->second << "\n";
    activeAlerts.erase(it);
}

void CommunicationsUnit::onIncidentCancelled(const std::string& incidentId)
{
    const auto it = activeAlerts.find(incidentId);
    if (it == activeAlerts.end())
    {
        return;
    }

    if (alarmSystem != nullptr)
    {
        alarmSystem->silenceAlert(it->second);
    }

    std::cout << "[Communications " << id << "] Alert silenced at " << it->second << "\n";
    activeAlerts.erase(it);
}

void CommunicationsUnit::onAreaSecured(const std::string& location)
{
    std::cout << "[Communications " << id << "] Area-secured update received for " << location << "\n";
}

void CommunicationsUnit::onEmergencyAlert(const std::string& message)
{
    std::cout << "[Communications " << id << "] Emergency broadcast: " << message << "\n";
}
