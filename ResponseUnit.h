#ifndef RESPONSEUNIT_H
#define RESPONSEUNIT_H

#include <string>

class ControlRoom;

class ResponseUnit
{
protected:
    std::string id;
    ControlRoom *controlRoom;

public:
    ResponseUnit(const std::string &id);

    virtual ~ResponseUnit() = default;

    std::string getId() const;

    void setControlRoom(ControlRoom *room);

    virtual std::string getUnitType() const = 0;

    virtual void onIncidentReported(const std::string &incidentId, const std::string &location, int severity)
    {
    }

    virtual void onIncidentDispatched(const std::string &incidentId, const std::string &location, const std::string &unitType, int severity)
    {
    }

    virtual void onIncidentResolved(const std::string &incidentId, const std::string &unitType)
    {
    }

    virtual void onIncidentCancelled(const std::string &incidentId)
    {
    }

    virtual void onAreaSecured(const std::string &location)
    {
    }

    virtual void onHazardDetected(const std::string &location, const std::string &hazard)
    {
    }

    virtual void enterArea(const std::string &location)
    {
    }
};

#endif
