#include "PSerializer.h"

using namespace Pt::Core;


DeclStart_SerializeFunctions(qint64) =
{
    Decl_SerializeFunctionEntry(Normal, qint64, return QString::number(value);)
};

DeclStart_DeserializeFunctions(qint64) =
{
    Decl_DeserializeFunctionEntry(Normal, qint64, return data.toLongLong();)
};

DeclStart_SerializeFunctions(QString) =
{
    Decl_SerializeFunctionEntry(Normal, QString, return value;)
};

DeclStart_DeserializeFunctions(QString) =
{
    Decl_DeserializeFunctionEntry(Normal, QString, return data;)
};

DeclStart_SerializeFunctions(cstring) =
{
    Decl_SerializeFunctionEntry(Normal, cstring, return QString::fromUtf8(value); )
};

DeclStart_DeserializeFunctions(cstring) =
{
    Decl_DeserializeFunctionEntry(Normal, cstring, return data.toUtf8().data(); )
};

//Decl_SerializeFunction(qint64, return QString::number(t); )
//Decl_SerializeFunctionItem(qint64, Object, return QString::number(t); )
//Decl_DeserializeFunction(qint64, return s.toLongLong(); )

//Decl_SerializeFunction(QString, return t; )
//Decl_DeserializeFunction(QString, return s; )
