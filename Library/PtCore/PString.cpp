#include "PString.h"
#include "PSerializer.h"

namespace Pt {
namespace Core {
PString::PString()
{
}

QString PString::format(QString pattern, const PStringPairList &parameters)
{
    for (auto parameter : parameters)
        pattern.replace(QString("{%1}").arg(parameter.first), parameter.second);
    return pattern;
}

QString PStringMap::serialize(const PStringMap &value)
{
    return PSerializer::serialize(QMap<QString, QString>(value));
}

DeclStart_SerializeFunctions(PStringMap) =
{
        DeclEntry_SerializeFunction(Normal, PStringMap, return PStringMap::serialize(value); )
};

}
}
