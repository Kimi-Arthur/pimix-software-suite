#include "PSerializer.h"

Decl_SerializeFunction(qint64, return QString::number(t); )
Decl_DeserializeFunction(qint64, return s.toLongLong(); )

Decl_SerializeFunction(QString, return t; )
Decl_DeserializeFunction(QString, return s; )
