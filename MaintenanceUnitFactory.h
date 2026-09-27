#ifndef MAINTENANCEUNITFACTORY_H
#define MAINTENANCEUNITFACTORY_H

#include "ResponseUnitFactory.h"

class MaintenanceUnitFactory : public ResponseUnitFactory
{
public:
    std::string factoryType() const override;

    std::unique_ptr<ResponseUnit>
    createUnit(const std::string& id) const override;
};

#endif
