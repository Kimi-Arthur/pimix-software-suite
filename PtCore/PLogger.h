#ifndef PLOGGER_H
#define PLOGGER_H

#include <QMap>
#include <QString>

class PLogger
{
public:
    enum LogLevel {
        DebugLog, InformationLog, WarningLog, ErrorLog, CriticalLog
    };

    QMap<LogLevel, QString> logStrings;
    PLogger();
};

#endif // PLOGGER_H
