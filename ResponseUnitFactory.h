#ifndef RESPONSEUNITFACTORY_H
#define RESPONSEUNITFACTORY_H

#include <memory>
#include <string>

class ResponseUnit;

class ResponseUnitFactory
{
public:
    virtual ~ResponseUnitFactory() = default;

    virtual std::string factoryType() const = 0;

    virtual std::unique_ptr<ResponseUnit>
    createUnit(const std::string& id) const = 0;
};

#endif
