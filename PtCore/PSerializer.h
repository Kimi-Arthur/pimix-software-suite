#ifndef PSERIALIZER_H
#define PSERIALIZER_H

#include <QString>
#include <QMap>
#include <functional>

namespace Pt {
namespace Core {

template<class T>
class EnumSerializer
{
    static QMap<QString, T> fromStringMap;
    static QMap<T, QString> toStringMap;
public:
    static T fromString(QString s) {
        return fromStringMap.value(s);
    }
    static QString toString(T t) {
        return toStringMap.value(t);
    }
};

template<class T>
class PSerializer
{
    static std::function<T(QString)> fromStringFunction;
    static std::function<QString(T)> toStringFunction;
public:
    PSerializer();
    static inline QString toString(T arg)
    {
        return toStringFunction(arg);
    }

    static inline T fromString(QString arg)
    {
        return fromStringFunction(arg);
    }
};


#define Decl_Enum_Serializer(T) \
    template<> std::function<T(QString)> PSerializer<T>::fromStringFunction = [](QString s) { return EnumSerializer<T>::fromString(s) }; \
    template<> std::function<QString(T)> PSerializer<T>::toStringFunction = [](T t) { return EnumSerializer<T>::toString(s) };

}
}

#endif // PSERIALIZER_H
