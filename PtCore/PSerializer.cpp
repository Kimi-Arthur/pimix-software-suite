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

DeclStart_SerializeFunctions(QLatin1String) =
{
    Decl_SerializeFunctionEntry(Normal, QLatin1String, return value;)
};

DeclStart_DeserializeFunctions(QLatin1String) =
{
    Decl_DeserializeFunctionEntry(Normal, QLatin1String, return QLatin1String(data.toLatin1());)
};


//Decl_SerializeFunction(qint64, return QString::number(t); )
//Decl_SerializeFunctionItem(qint64, Object, return QString::number(t); )
//Decl_DeserializeFunction(qint64, return s.toLongLong(); )

//Decl_SerializeFunction(QString, return t; )
//Decl_DeserializeFunction(QString, return s; )
