#ifndef PTCORE_H
#define PTCORE_H

#include "ptcore_global.h"

#include <QDateTime>

namespace Pt {

namespace Core {

template <typename T> inline const T &pCeilDiv(const T &a, const T &b) { return (a - 1) / b + 1; }

QString PTCORESHARED_EXPORT dateTimeToString(QDateTime dt, Qt::DateFormat format, QString seperator=" ");

QString formatString(QString pattern, QList<QPair<QString, QString> > parameters);
}
}

#endif // PTCORE_H
