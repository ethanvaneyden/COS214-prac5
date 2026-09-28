#include "SecurityUnitFactory.h"
#include "SecurityUnit.h"

std::string SecurityUnitFactory::factoryType() const
{
    return "security";
}

std::unique_ptr<ResponseUnit>
SecurityUnitFactory::createUnit(const std::string& id) const
{
    return std::unique_ptr<ResponseUnit>(new SecurityUnit(id));
}
