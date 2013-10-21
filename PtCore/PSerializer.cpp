#include "PSerializer.h"

template<>
std::function<qint64(QString)> Pt::Core::PSerializer<qint64>::fromStringFunction = [](QString s) { return s.toLongLong(); };

template<>
std::function<QString(qint64)> Pt::Core::PSerializer<qint64>::toStringFunction = [](qint64 t) { return QString::number(t); };
