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

    template<class T>
    static inline QString serialize(const QList<T> &value, PSerializationType serializationType = PSerializationType::Normal)
    {
        QString result = "[";
        foreach (auto &v, value) {
            result += PSerializer::serialize(v, serializationType) + ",";
        }
        result.chop(1);
        result.append("]");

        return result;
    }

    static inline QString serialize(const QStringList value, PSerializationType serializationType = PSerializationType::Normal)
    {
        return PSerializer::serialize(QList<QString>(value));
    }

    static inline QString serialize(const char *value, PSerializationType serializationType = PSerializationType::Normal)
    {
        return value;
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

#define DeclEntry_SerializeFunction(ST, T, C...) \
    {Pt::Core::PSerializationType::ST, [](const T &value) -> QString { C } }

#define DeclEntry_DeserializeFunction(ST, T, C...) \
    {Pt::Core::PSerializationType::ST, [](const QString &data) -> T { C } }

// EnumSerialization

template<class T>
class PTCORESHARED_EXPORT EnumSerializer
{
public:
    static QMap<QString, T> stringToValueMap;
    static QMap<T, QString> valueToStringMap;
};

#define DeclStart_EnumSTVM(T) \
    template<> \
    QMap<QString, T> EnumSerializer<T>::stringToValueMap

#define DeclEntry_EnumSTVM(T, V) \
    {#V, T::V}

#define DeclStart_EnumVTSM(T) \
    template<> \
    QMap<T, QString> EnumSerializer<T>::valueToStringMap

#define DeclEntry_EnumVTSM(T, V) \
    {T::V, #V}

#define Decl_Enum_Serializer(T) \
    DeclStart_SerializeFunctions(T) = { \
        DeclEntry_SerializeFunction(Normal, T, return Pt::Core::EnumSerializer<T>::valueToStringMap.value(value); ) \
    }; \
    DeclStart_DeserializeFunctions(T) = { \
        DeclEntry_DeserializeFunction(Normal, T, return Pt::Core::EnumSerializer<T>::stringToValueMap.value(data); ) \
    };

}
}


#endif // PSERIALIZER_H
