#ifndef PTCORE_H
#define PTCORE_H

#include "ptcore_global.h"

#include <QDateTime>

namespace Pt {

template <typename T> inline const T &pCeilDiv(const T &a, const T &b) { return (a - 1) / b + 1; }

QString PTCORESHARED_EXPORT dateTimeToString(QDateTime dt, Qt::DateFormat format, QString seperator=" ");

}

class PTCORESHARED_EXPORT PtCore
{
    
public:
    PtCore();
};

#endif // PTCORE_H
