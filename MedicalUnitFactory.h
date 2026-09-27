#ifndef MEDICALUNITFACTORY_H
#define MEDICALUNITFACTORY_H

#include "ResponseUnitFactory.h"

class MedicalUnitFactory : public ResponseUnitFactory
{
public:
    std::string factoryType() const override;

    std::unique_ptr<ResponseUnit>
    createUnit(const std::string& id) const override;
};

#endif
