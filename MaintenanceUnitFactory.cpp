#include "MaintenanceUnitFactory.h"
#include "MaintenanceUnit.h"

std::string MaintenanceUnitFactory::factoryType() const
{
    return "maintenance";
}

std::unique_ptr<ResponseUnit>
MaintenanceUnitFactory::createUnit(const std::string& id) const
{
    return std::unique_ptr<ResponseUnit>(new MaintenanceUnit(id));
}
