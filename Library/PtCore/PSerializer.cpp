#include "PSerializer.h"
#include <QStringList>
#include "PJsonValue.h"

using namespace Pt::Core;


DeclStart_SerializeFunctions(qint64) =
{
    DeclEntry_SerializeFunction(Normal, qint64, return QString::number(value); )
};

DeclStart_DeserializeFunctions(qint64) =
{
    DeclEntry_DeserializeFunction(Normal, qint64, return data.toLongLong(); )
};

DeclStart_SerializeFunctions(int) =
{
    DeclEntry_SerializeFunction(Normal, int, return QString::number(value);),
    DeclEntry_SerializeFunction(JsonFormat, int, return PJsonValue::serialize(PJsonValue(value)); )
};

DeclStart_DeserializeFunctions(int) =
{
    DeclEntry_DeserializeFunction(Normal, int, return data.toInt();),
    DeclEntry_DeserializeFunction(JsonFormat, int, return PJsonValue::deserialize(data).toInt(); )
};

DeclStart_SerializeFunctions(bool) =
{
    DeclEntry_SerializeFunction(Normal, bool, return value ? "true" : "false"; ),
    DeclEntry_SerializeFunction(JsonFormat, bool, return PJsonValue::serialize(PJsonValue(value)); )
};

DeclStart_DeserializeFunctions(bool) =
{
    DeclEntry_DeserializeFunction(Normal, bool, return data == "true"; ),
    DeclEntry_DeserializeFunction(JsonFormat, bool, return PJsonValue::deserialize(data).toBool(); )
};

DeclStart_SerializeFunctions(QString) =
{
    DeclEntry_SerializeFunction(Normal, QString, return value;)
};

DeclStart_DeserializeFunctions(QString) =
{
    DeclEntry_DeserializeFunction(Normal, QString, return data;)
};

DeclStart_DeserializeFunctions(char *) =
{
    DeclEntry_DeserializeFunction(Normal, char *, return data.toUtf8().data();)
};

DeclStart_EnumSTVM(Pt::Core::ResultType) =
{
    DeclEntry_EnumSTVM(Pt::Core::ResultType, Success),
    DeclEntry_EnumSTVM(Pt::Core::ResultType, Failure)
};

DeclStart_EnumVTSM(Pt::Core::ResultType) =
{
    DeclEntry_EnumVTSM(Pt::Core::ResultType, Success),
    DeclEntry_EnumVTSM(Pt::Core::ResultType, Failure)
};

Decl_Enum_Serializer(Pt::Core::ResultType)
