#include "MedicalUnitFactory.h"
#include "MedicalUnit.h"

std::string MedicalUnitFactory::factoryType() const
{
    return "medical";
}

std::unique_ptr<ResponseUnit>
MedicalUnitFactory::createUnit(const std::string& id) const
{
    return std::unique_ptr<ResponseUnit>(new MedicalUnit(id));
}
