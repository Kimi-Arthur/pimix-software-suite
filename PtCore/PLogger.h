#ifndef PLOGGER_H
#define PLOGGER_H

#include <QMap>
#include <QSet>
#include <QString>
#include <QDateTime>
#include "ptcore_global.h"
#include <QPluginLoader>

class PTCORESHARED_EXPORT PLogger
{
public:
    // Public Types
    enum LogType {
        DebugLog, InformationLog, WarningLog, ErrorLog, FatalLog
    };

    // Public Constants
    const QMap<LogType, QString> LogStrings = {{DebugLog, "DEBUG"}, {InformationLog, "INFO"}, {WarningLog, "WARN"}, {ErrorLog, "ERROR"}, {FatalLog, "FATAL"}};
    const QString DefaultLogPattern = "[{time}][{type}]{content}";
    const QString DefaultLogFileNamePattern = "{base_path}/{iid}-{date}";
    const LogType DefaultDisplayBound = InformationLog;

    // Public Data Memebers
    QString logPattern; // What the logs contain.
    LogType displayBound; // Which logs will be displayed.
    QMap<LogType, QSet<QString>> logFileNamePattern; // What the log files' names are.
    QMap<QString, QString> generalParamters; // Customized fill-in parameters.

    // Public Constructors
    PLogger();
    PLogger(QPluginLoader &loader);

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
