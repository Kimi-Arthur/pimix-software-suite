#ifndef PTCORE_H
#define PTCORE_H

#include "PtCore_global.h"

#include <QDateTime>

namespace Pt {

namespace Core {

enum class ResultType {
    Success, Failure
};

template <typename T> inline const T &pCeilDiv(const T &a, const T &b) { return (a - 1) / b + 1; }

QString PTCORESHARED_EXPORT dateTimeToString(QDateTime dt, Qt::DateFormat format, QString seperator=" ");

}
}

#endif // PTCORE_H
