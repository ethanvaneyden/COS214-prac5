#include "IncidentRegistry.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

IncidentRegistry::IncidentRegistry()
{
    incidentNumber = 1;
}

Incident *IncidentRegistry::createIncident(string location, string description, string type, int severity, ControlRoom *room)
{
    if (severity < 1 || severity > 4)
    {
        cout << "Invalid severity\n";
        return nullptr;
    }

    ostringstream oss;
    oss << "inc-" << setw(3) << setfill('0') << incidentNumber++;
    string incidentId = oss.str();

    unique_ptr<Incident> incident = unique_ptr<Incident>(new Incident(incidentId, location, description, type, static_cast<Incident::Severity>(severity), room));

    Incident *raw = incident.get();
    registry.emplace(incidentId, move(incident));

    return raw;
}

Incident *IncidentRegistry::find(const string &id) const
{
    auto it = registry.find(id);
    if (it != registry.end())
    {
        return (it->second).get();
    }
    else
    {
        return nullptr;
    }
}

void IncidentRegistry::printIncidents() const
{
    auto it = registry.begin();
    while (it != registry.end())
    {
        const auto &incident = it->second;
        std::cout << "  " << incident->getId()
                  << "  [" << incident->getStateName() << "]"
                  << "  " << incident->getType()
                  << " @ " << incident->getLocation()
                  << "  (sev " << incident->getSeverityNumber() << ")\n";
        it++;
    }
}
