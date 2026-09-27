
#include "ControlRoom.h"
#include "ResponseUnit.h"
#include "ResponseUnitFactory.h"
#include <iostream>

using namespace std;

void ControlRoom::incidentReported(const string &id, const string &location, int severity)
{
    for (const auto &unit : registeredUnits)
    {
        unit->onIncidentReported(id, location, severity);
    }
}

void ControlRoom::incidentDispatched(const string &id, const string &location, const string &unitType, int severity)
{
    for (const auto &unit : registeredUnits)
    {
        unit->onIncidentDispatched(id, location, unitType, severity);
    }

    if (severity >= 4)
    {
        areaSecured(location);
    }
}

void ControlRoom::incidentResolved(const string &id, const string &unitType)
{
    for (const auto &unit : registeredUnits)
    {
        unit->onIncidentResolved(id, unitType);
    }
}

void ControlRoom::incidentCancelled(const string &id)
{
    for (const auto &unit : registeredUnits)
    {
        unit->onIncidentCancelled(id);
    }
}

void ControlRoom::areaSecured(const string &location)
{
    for (const auto &unit : registeredUnits)
    {
        unit->onAreaSecured(location);
    }
}

void ControlRoom::emergencyAlert(const string &message)
{
    for (const auto &unit : registeredUnits)
    {
        unit->onEmergencyAlert(message);
    }
}

void ControlRoom::createUnit(const string &type, const string &id)
{
    auto it = factories.find(type);
    if (it != factories.end())
    {
        auto &factory = it->second;
        unique_ptr<ResponseUnit> unit = factory->createUnit(id);
        unit->setControlRoom(this);
        registeredUnits.push_back(move(unit));
        cout << "[Control Room] registered " << unit->getId() << "\n";
    }
    else
    {
        cout << "Can't create that unit!\n";
    }
}

ResponseUnit *ControlRoom::findUnit(const string &unitId) const
{
    for (const auto &unit : registeredUnits)
    {
        if (unit->getId() == unitId)
        {
            return unit.get();
        }
    }
    return nullptr;
}

void ControlRoom::hazardDetected(const string &location, const string &hazard)
{
    for (const auto &unit : registeredUnits)
    {
        unit->onHazardDetected(location, hazard);
    }
}

void ControlRoom::entryRequested(const string &location, const string &requesterId)
{
    for (const auto &unit : registeredUnits)
    {
        unit->onEntryRequested(location, requesterId);
    }
}

void ControlRoom::listUnits() const
{
    if (registeredUnits.empty())
    {
        cout << "[Desk] no units!\n";
        return;
    }

    for (const auto &unit : registeredUnits)
    {

        cout << "  " << unit->getId()
             << " (" << unit->getUnitType() << ")\n";
    }
}

void ControlRoom::registerFactory(string type, unique_ptr<ResponseUnitFactory> factory)
{
    factories.emplace(type, move(factory));
}