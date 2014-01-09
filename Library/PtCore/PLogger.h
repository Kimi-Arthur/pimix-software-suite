#ifndef PLOGGER_H
#define PLOGGER_H

#include "PtCore_global.h"
#include "PSerializer.h"
#include "PString.h"

namespace Pt {
namespace Core {

#ifdef __FUNCSIG__
  #define __PFUNC_ID__ __FUNCSIG__
#else
  #define __PFUNC_ID__ __PRETTY_FUNCTION__
#endif


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

    /**
     * Centric log process logic
     *
     * @param content Main log content
     * @param objectName If it's not empty, the content will be displayed like "objectName = Type(content)"
     * @param logType The log's type, similar to log level.
     */
    template<class T>
    void log(const T &content, const QString &objectName = "", LogType logType = LogType::TraceLog) const
    {
        QDateTime cdt = QDateTime::currentDateTime();
        PStringMap parameters = {
            {"content", PSerializer::serialize(content)},
            {"type", LogTypeStrings[logType]},
            {"datetime", cdt.toOffsetFromUtc(cdt.offsetFromUtc()).toString(Qt::ISODate)}
        };

        if (objectName != "")
            parameters["content"].prepend(objectName + QSL(" = "));

        QString resultLog = PString::format(logPattern, parameters);


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

    inline void logMethodIn(const QString &functionId)
    {
        trace(">>> " + functionId);
    }

    inline void logMethodOut(const QString &functionId)
    {
        trace("<<< " + functionId);
    }

    template<class T>
    inline void logAssertEquals(const T &expected, const T &actual, const QString &objectName = "") const
    {
        QString content = PSerializer::serialize(actual) + " (" + PSerializer::serialize(expected) + ")";
        if (expected == actual)
            debug(content, objectName);
        else
            warn(content, objectName);
    }

private:
    const QMap<LogType, QString> LogTypeStrings = {{LogType::TraceLog, "TRACE"}, {LogType::DebugLog, "DEBUG"},
                                                   {LogType::InformationLog, "INFO "}, {LogType::WarningLog, "WARN "},
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
