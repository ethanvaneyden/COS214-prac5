#ifndef INCIDENTREGISTRY_H
#define INCIDENTREGISTRY_H

#include "Incident.h"
#include <map>
#include <memory>
#include <string>

class ControlRoom;

/**
 * @brief Owns all reported incidents
 *
 */
class IncidentRegistry
{

private:
    std::map<std::string, std::unique_ptr<Incident>> registry;
    int incidentNumber;

public:
    IncidentRegistry();
    ~IncidentRegistry() = default;

    Incident* createIncident(std::string location, std::string description, std::string type, int severity, ControlRoom *room);
    Incident* find(std::string id);
    void printIncidents() const;
};

#endif