#include "PConfig.h"

Pt::Core::PConfig::PConfig()
{
}


void Pt::Core::PConfig::setJsonValue(const PJsonValue &value)
{

}

PJsonValue Pt::Core::PConfig::getJsonValue() const
{
    PJsonValue result;
    for (auto parameter : data)
        result.setValue(parameter.first, parameter.second);
    return result;
}

void Pt::Core::PConfig::setValue(const QString &jsonPath, const PJsonValue &value)
{
    data[jsonPath] = value;
}

PJsonValue Pt::Core::PConfig::getValue(const QString &jsonPath) const
{
    return data[jsonPath];
}
