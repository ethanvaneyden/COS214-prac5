#include "CommunicationsUnitFactory.h"
#include "CommunicationsUnit.h"

CommunicationsUnitFactory::CommunicationsUnitFactory(AlarmSystem* alarmSystem)
    : alarmSystem(alarmSystem)
{
}

std::string CommunicationsUnitFactory::factoryType() const
{
    return "comms";
}

std::unique_ptr<ResponseUnit>
CommunicationsUnitFactory::createUnit(const std::string& id) const
{
    return std::unique_ptr<ResponseUnit>(new CommunicationsUnit(id, alarmSystem));
}
