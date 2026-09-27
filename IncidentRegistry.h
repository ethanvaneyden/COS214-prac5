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

    /**
     * @brief Create a Incident object
     * 
     * @param location 
     * @param description 
     * @param type 
     * @param severity 
     * @param room 
     * @return Incident* 
     */
    Incident* createIncident(std::string location, std::string description, std::string type, int severity, ControlRoom *room);
    /**
     * @brief Returns the incident if it exists else returns nullptr
     * 
     * @param id 
     * @return Incident* 
     */
    Incident* find(std::string id);
    /**
     * @brief Prints all incidents in id order
     * 
     */
    void printIncidents() const;
};

#endif