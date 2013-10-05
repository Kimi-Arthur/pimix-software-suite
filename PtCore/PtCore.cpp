#include "PtCore.h"

#include <QDebug>


QString Pt::Core::dateTimeToString(QDateTime dt, Qt::DateFormat format, QString separator)
{
    if (format != Qt::ISODate)
        return dt.toString(format);
    QDateTime tempDt(dt);
    tempDt.setTimeSpec(Qt::UTC);
    dt.setUtcOffset(dt.secsTo(tempDt));
    return dt.toString(Qt::ISODate).replace("T", separator);
}


QString Pt::Core::formatString(QString pattern, QList<QPair<QString, QString> > parameters)
{

}
