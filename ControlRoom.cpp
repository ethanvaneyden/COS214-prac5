
#include "ControlRoom.h"
#include <iostream>

using namespace std;

ControlRoom::ControlRoom()
{
    // Todo: Implement
}

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

void emergencyAlert(const string &message)
{
}

void ControlRoom::createUnit(const string &type, const string &id)
{
    auto it = factories.find(type);
    if (it != factories.end())
    {
        auto &factory = it->second;
        factory->createUnit(type, id);
    }
    else
    {
        cout << "Can't create that unit!\n";
    }
}

ResponseUnit *findUnit(const string &unitId) const
{
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
    for(const auto &unit : registeredUnits) {
        if(unit->getId() == requesterId) {
            unit->enterArea(location);
        }
    }
}

void ControlRoom::listUnits() const {
    //TODO: Implement method
}

void ControlRoom::registerFactory(string type, unique_ptr<ResponseUnitFactory> factory) {
    factories.emplace(type, move(factory));
}