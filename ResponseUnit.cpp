#include "ResponseUnit.h"

ResponseUnit::ResponseUnit(const std::string& id)
    : id(id), controlRoom(nullptr)
{
}

std::string ResponseUnit::getId() const
{
    return id;
}

void ResponseUnit::setControlRoom(ControlRoom* room)
{
    controlRoom = room;
}
