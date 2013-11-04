#ifndef PSERIALIZER_H
#define PSERIALIZER_H

#include "PtCore.h"
#include <QDebug>

namespace Pt {
namespace Core {

enum class PSerializationType
{
    Normal, LongFormat
};

template<class T>
class PTCORESHARED_EXPORT PSerializerInformation
{
public:
    static QMap<PSerializationType, std::function<QString(const T&)>> serializeFunctions;
    static QMap<PSerializationType, std::function<T(const QString&)>> deserializeFunctions;
};

class PTCORESHARED_EXPORT PSerializer
{
public:
    template<class T>
    static inline QString serialize(const T &value, PSerializationType serializationType = PSerializationType::Normal)
    {
        return PSerializerInformation<T>::serializeFunctions.value(serializationType)(value);
    }

    static inline QString serialize(const char *value, PSerializationType serializationType = PSerializationType::Normal)
    {
        return value;
    }

    template<class T>
    static inline T deserialize(const QString &data, PSerializationType serializationType = PSerializationType::Normal)
    {
        return PSerializerInformation<T>::deserializeFunctions.value(serializationType)(data);
    }
};

#define DeclStart_SerializeFunctions(T) \
    template<> \
    QMap<Pt::Core::PSerializationType, std::function<QString(const T&)>> \
    Pt::Core::PSerializerInformation<T>::serializeFunctions

#define DeclStart_DeserializeFunctions(T) \
    template<> \
    QMap<Pt::Core::PSerializationType, std::function<T(const QString&)>> \
    Pt::Core::PSerializerInformation<T>::deserializeFunctions

#define Decl_SerializeFunctionEntry(ST, T, C...) \
    {Pt::Core::PSerializationType::ST, [](const T &value) -> QString { C } }

#define Decl_DeserializeFunctionEntry(ST, T, C...) \
    {Pt::Core::PSerializationType::ST, [](const QString &data) -> T { C } }

// EnumSerialization

template<class T>
class PTCORESHARED_EXPORT EnumSerializer
{
public:
    static QMap<QString, T> stringToValueMap;
    static QMap<T, QString> valueToStringMap;
};

#define Decl_Enum_Serializer(T) \
    Decl_DeserializeFunction(T, return Pt::Core::EnumSerializer<T>::stringToValueMap.value(s); ) \
    Decl_SerializeFunction(T, return Pt::Core::EnumSerializer<T>::valueToStringMap.value(s); )
}
}


#endif // PSERIALIZER_H
