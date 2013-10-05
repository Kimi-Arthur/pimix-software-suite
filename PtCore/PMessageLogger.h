#ifndef PLOGGER_H
#define PLOGGER_H

#include <QMap>
#include <QSet>
#include <QString>
#include <QDateTime>
#include "ptcore_global.h"
#include <QPluginLoader>

namespace Pt {
namespace Core {

class PTCORESHARED_EXPORT PMessageLogger
{
    static PMessageLogger *staticInstance;
public:
    static PMessageLogger *globalInstance();
    // Public Types
    enum class LogType {
        TraceLog, DebugLog, InformationLog, WarningLog, ErrorLog, FatalLog
    };

    // Public Constants
    const QMap<LogType, QString> LogStrings = {{LogType::TraceLog, "TRACE"}, {LogType::DebugLog, "DEBUG"},
                                               {LogType::InformationLog, "INFO"}, {LogType::WarningLog, "WARN"},
                                               {LogType::ErrorLog, "ERROR"}, {LogType::FatalLog, "FATAL"}};
    const QString DefaultLogPattern = "[{datetime}][{type}]{content}";
    const QString DefaultLogFileNamePattern = "{base_path}/{date}/{iid}-{pid}-{time}";
    const LogType DefaultDisplayBound = LogType::InformationLog;

    // Public Data Memebers
    QString logPattern; // What the logs contain.
    LogType displayBound; // Which logs will be displayed.
    QMap<LogType, QSet<QString>> logFileNamePattern; // What the log files' names are.
    QMap<QString, QString> generalParamters; // Customized fill-in parameters.

    // Public Constructors
    PMessageLogger();
    PMessageLogger(QPluginLoader &loader);

    // Public Methods
    void log(QString content, LogType logType = LogType::TraceLog) const;
    void debug(QString content) const;
    void info(QString content) const;
    void warn(QString content) const;
    void error(QString content) const;
    void fatal(QString content) const;

private:
    void displayLog(QString resultLog, LogType logType) const;
    void writeLog(QString resultLog, LogType logType) const;
};

}
}

#endif // PLOGGER_H
