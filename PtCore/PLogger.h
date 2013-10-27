#ifndef PLOGGER_H
#define PLOGGER_H

#include <QMap>
#include <QSet>
#include <QString>
#include <QDateTime>
#include <QTextStream>
#include "ptcore_global.h"
#include "PSerializer.h"

namespace Pt {
namespace Core {

class PTCORESHARED_EXPORT PLogger
{
    static PLogger *staticInstance;
public:
    static PLogger *globalInstance();
    // Public Types
    enum class LogType {
        TraceLog, DebugLog, InformationLog, WarningLog, ErrorLog, FatalLog
    };

    // Public Constants
    const QString DefaultLogPattern = "[{datetime}][{type}] {content}";
    const QString DefaultLogFileNamePattern = "{base_path}/{date}/{iid}-{pid}-{time}";
    const LogType DefaultDisplayBound = LogType::InformationLog;

    // Public Data Memebers
    QString logPattern; // What the logs contain.
    LogType displayBound; // Which logs will be displayed.
    QMap<LogType, QSet<QString>> logFileNamePattern; // What the log files' names are.
    QMap<QString, QString> generalParamters; // Customized fill-in parameters.

    // Public Constructors
    PLogger();

    // Public Methods
    template<class T>
    void log(const T &content, const QString &objectName = "", LogType logType = LogType::TraceLog) const
    {
        std::map<QString, QString> parameters = {
            {"content", PSerializer::serialize(content)},
            {"type", LogTypeStrings[logType]},
            {"datetime", QDateTime::currentDateTime().toString(Qt::ISODate)}
        };
        QString resultLog = logPattern;
        foreach (auto parameter, parameters)
            resultLog.replace("{" + parameter.first + "}", parameter.second);

        displayLog(resultLog, logType);
        writeLog(resultLog, logType);
    }

    template<class T>
    inline void trace(const T &content, const QString &objectName = "") const
    {
        log(content, objectName, LogType::TraceLog);
    }

    template<class T>
    inline void debug(const T &content, const QString &objectName = "") const
    {
        log(content, objectName, LogType::DebugLog);
    }

    template<class T>
    inline void info(const T &content, const QString &objectName = "") const
    {
        log(content, objectName, LogType::InformationLog);
    }

    template<class T>
    inline void warn(const T &content, const QString &objectName = "") const
    {
        log(content, objectName, LogType::WarningLog);
    }

    template<class T>
    inline void error(const T &content, const QString &objectName = "") const
    {
        log(content, objectName, LogType::ErrorLog);
    }

    template<class T>
    inline void fatal(const T &content, const QString &objectName = "") const
    {
        log(content, objectName, LogType::FatalLog);
    }

private:
    const QMap<LogType, QString> LogTypeStrings = {{LogType::TraceLog, "TRACE"}, {LogType::DebugLog, "DEBUG"},
                                                   {LogType::InformationLog, "INFO"}, {LogType::WarningLog, "WARN"},
                                                   {LogType::ErrorLog, "ERROR"}, {LogType::FatalLog, "FATAL"}};
    const QString ListSeparator = ", ";
    template<class T>
    QString objectToString(const QList<T> &objectList);
    void displayLog(QString resultLog, LogType logType) const;
    void writeLog(QString resultLog, LogType logType) const;
};

}
}

#endif // PLOGGER_H
