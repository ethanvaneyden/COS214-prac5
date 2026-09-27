#ifndef SECURITYUNITFACTORY_H
#define SECURITYUNITFACTORY_H

#include "ResponseUnitFactory.h"

class SecurityUnitFactory : public ResponseUnitFactory
{
public:
    std::string factoryType() const override;

    std::unique_ptr<ResponseUnit>
    createUnit(const std::string& id) const override;
};

#endif
