#include "PSerializer.h"
#include <QStringList>

using namespace Pt::Core;


DeclStart_SerializeFunctions(qint64) =
{
    Decl_SerializeFunctionEntry(Normal, qint64, return QString::number(value);)
};

DeclStart_DeserializeFunctions(qint64) =
{
    Decl_DeserializeFunctionEntry(Normal, qint64, return data.toLongLong();)
};

DeclStart_SerializeFunctions(int) =
{
    Decl_SerializeFunctionEntry(Normal, int, return QString::number(value);)
};

DeclStart_DeserializeFunctions(int) =
{
    Decl_DeserializeFunctionEntry(Normal, int, return data.toInt();)
};

DeclStart_SerializeFunctions(QString) =
{
    Decl_SerializeFunctionEntry(Normal, QString, return value;)
};

DeclStart_DeserializeFunctions(QString) =
{
    Decl_DeserializeFunctionEntry(Normal, QString, return data;)
};

DeclStart_DeserializeFunctions(char *) =
{
    Decl_DeserializeFunctionEntry(Normal, char *, return data.toUtf8().data();)
};

template<>
QMap<Pt::Core::PSerializationType, std::function<QString(const QStringList&)>>
Pt::Core::PSerializerInformation<QStringList>::serializeFunctions = {
{Pt::Core::PSerializationType::Normal, [](const QStringList &value) -> QString { return value.join("\n"); } }
};
