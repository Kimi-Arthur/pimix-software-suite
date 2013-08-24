#ifndef PLOGGER_H
#define PLOGGER_H

#include <QMap>
#include <QSet>
#include <QString>
#include <QDateTime>

class PLogger
{
public:
    PLogger();
    // Public Types
    enum LogType {
        DebugLog, InformationLog, WarningLog, ErrorLog, FatalLog
    };

    // Public Constants
    const QMap<LogType, QString> LogStrings = {{DebugLog, "DEBUG"}, {InformationLog, "INFO"}, {WarningLog, "WARN"}, {ErrorLog, "ERROR"}, {FatalLog, "FATAL"}};
    const QString DefaultLogPattern = "[{time}][{type}]{content}";

    // Public Data Memebers
    QString logPattern;
    // Controls what log info will be displayed.
    LogType displayBound;

    QMap<LogType, QSet<QString>> logFileMapping;


    // Public Methods
    void log(QString content, LogType logType) const;
    void debug(QString content) const;
    void info(QString content) const;
    void warn(QString content) const;
    void error(QString content) const;
    void fatal(QString content) const;

private:
    void displayLog(QString resultLog, LogType logType) const;
    void writeLog(QString resultLog, LogType logType) const;
};

#endif // PLOGGER_H
