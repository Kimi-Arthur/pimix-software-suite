#ifndef PSERIALIZER_H
#define PSERIALIZER_H

#include <QString>
#include <QMap>
#include <functional>
#include "PtCore.h"

namespace Pt {
namespace Core {

template<class T>
class PTCORESHARED_EXPORT EnumSerializer
{
public:
    static QMap<QString, T> stringToValueMap;
    static QMap<T, QString> valueToStringMap;
};

template<class T>
class PTCORESHARED_EXPORT PSerializerInformation
{
public:
    static std::function<QString(const T&)> serializeFunction;
    static std::function<T(const QString&)> deserializeFunction;
};

class PTCORESHARED_EXPORT PSerializer
{
public:
    template<class T>
    static QString serialize(T value)
    {
        return PSerializerInformation<T>::serializeFunction(value);
    }

    template<class T>
    static T deserialize(QString str)
    {
        return PSerializerInformation<T>::deserializeFunction(str);
    }
};

#define Decl_SerializeFunction(T, C...) \
    template<> std::function<QString(const T&)> Pt::Core::PSerializerInformation<T>::serializeFunction = [](const T &t) { C };

#define Decl_DeserializeFunction(T, C...) \
    template<> std::function<T(const QString&)> Pt::Core::PSerializerInformation<T>::deserializeFunction = [](const QString &s) { C };


#define Decl_Enum_Serializer(T) \
    Decl_DeserializeFunction(T, return Pt::Core::EnumSerializer<T>::stringToValueMap.value(s); ) \
    Decl_SerializeFunction(T, return Pt::Core::EnumSerializer<T>::valueToStringMap.value(s); )
}
}

#endif // PSERIALIZER_H
