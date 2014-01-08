#include "PConfig.h"

namespace Pt {
namespace Core {

void PConfig::setJsonValue(const PJsonValue &value)
{

}

PJsonValue PConfig::getJsonValue() const
{
    PJsonValue result;
    for (auto parameter : data)
        result[parameter.first] = parameter.second;
    return result;
}

void PConfig::setValue(const QString &jsonPath, const PJsonValue &value)
{
    data[jsonPath] = value;
}

PJsonValue PConfig::getValue(const QString &jsonPath) const
{
    return data.at(jsonPath);
}

}
}
