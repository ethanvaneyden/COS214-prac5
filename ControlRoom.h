#ifndef CONTROLROOM_H
#define CONTROLROOM_H

#include <vector>
#include <memory>
#include <string>
class ResponseUnit;

class ControlRoom {

    private:
    std::vector<std::unique_ptr<ResponseUnit>> registeredUnits;

    public:
    virtual ~ControlRoom() = default;
    ControlRoom() = default;
    void registerUnit(std::unique_ptr<ResponseUnit> unit);
    void broadcastEmergency(ResponseUnit* sender, const std::string& msg);
};    

#endif