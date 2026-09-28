#include "MaintenanceUnit.h"
#include "ControlRoom.h"

#include <iostream>

MaintenanceUnit::MaintenanceUnit(const std::string& id)
    : ResponseUnit(id)
{
}

std::string MaintenanceUnit::getUnitType() const
{
    return "maintenance";
}

void MaintenanceUnit::onIncidentReported(
    const std::string& incidentId,
    const std::string& location,
    int severity)
{
    std::cout << "[Maintenance " << id
              << "] Incident " << incidentId
              << " reported at " << location
              << " (severity " << severity << ")\n";
}

void MaintenanceUnit::onIncidentDispatched(
    const std::string& incidentId,
    const std::string& location,
    const std::string& unitType,
    int severity)
{
    (void)unitType;
    (void)severity;

    std::cout << "[Maintenance " << id
              << "] Mobilising to " << location
              << " for incident " << incidentId
              << "\n";
}

void MaintenanceUnit::onHazardDetected(
    const std::string& location,
    const std::string& hazard)
{
    std::cout << "[Maintenance " << id
              << "] Handling " << hazard
              << " hazard at " << location
              << "\n";

    if (hazard == "gas")
    {
        std::cout << "[Maintenance " << id
                  << "] Gas mains shut off\n";
    }
    else
    {
        std::cout << "[Maintenance " << id
                  << "] Hazard made safe\n";
    }

    std::cout << "[Maintenance " << id
              << "] Hazard resolved\n";
}

void MaintenanceUnit::onIncidentResolved(
    const std::string& incidentId,
    const std::string& unitType)
{
    (void)unitType;

    std::cout << "[Maintenance " << id
              << "] Standing down after incident "
              << incidentId
              << " was resolved\n";
}

void MaintenanceUnit::onIncidentCancelled(
    const std::string& incidentId)
{
    std::cout << "[Maintenance " << id
              << "] Standing down after incident "
              << incidentId
              << " was cancelled\n";
}