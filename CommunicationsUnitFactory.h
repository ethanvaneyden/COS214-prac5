#ifndef COMMUNICATIONSUNITFACTORY_H
#define COMMUNICATIONSUNITFACTORY_H

#include "ResponseUnitFactory.h"

class AlarmSystem;

class CommunicationsUnitFactory : public ResponseUnitFactory
{
private:
    AlarmSystem* alarmSystem;

public:
    CommunicationsUnitFactory(AlarmSystem* alarmSystem);

    std::string factoryType() const override;

    std::unique_ptr<ResponseUnit>
    createUnit(const std::string& id) const override;
};

#endif
