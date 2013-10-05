#include "PMessageLogger.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>

Pt::Core::PMessageLogger *Pt::Core::PMessageLogger::staticInstance = nullptr;

Pt::Core::PMessageLogger *Pt::Core::PMessageLogger::globalInstance()
{
    if (staticInstance == nullptr)
        staticInstance = new PMessageLogger();
    return staticInstance;
}

Pt::Core::PMessageLogger::PMessageLogger()
{
    logPattern = DefaultLogPattern;
    for (LogType t = LogType::InformationLog; t <= LogType::FatalLog; t = static_cast<LogType>(static_cast<int>(t) + 1))
        logFileNamePattern.insert(t, {DefaultLogFileNamePattern + ".log"});
    logFileNamePattern.insert(LogType::DebugLog, {DefaultLogFileNamePattern + "-debug.log"});
    logFileNamePattern.insert(LogType::DebugLog, {DefaultLogFileNamePattern + "-full.log"});
    logFileNamePattern.insert(LogType::DebugLog, {DefaultLogFileNamePattern + "-full.log"});
    displayBound = DefaultDisplayBound;
}

Pt::Core::PMessageLogger::PMessageLogger(QPluginLoader &loader) :
    PMessageLogger()
{

}


void Pt::Core::PMessageLogger::log(QString content, LogType logType) const
{
    std::map<QString, QString> parameters = {
        {"content", content},
        {"type", LogStrings[logType]},
        {"time", QDateTime::currentDateTime().toString(Qt::ISODate)}
    };
    QString resultLog = logPattern;
    foreach (auto parameter, parameters)
        resultLog.replace("{" + parameter.first + "}", parameter.second);

    displayLog(resultLog, logType);
    writeLog(resultLog, logType);
}

void Pt::Core::PMessageLogger::debug(QString content) const
{
    log(content, LogType::DebugLog);
}

void Pt::Core::PMessageLogger::info(QString content) const
{
    log(content, LogType::InformationLog);
}

void Pt::Core::PMessageLogger::warn(QString content) const
{
    log(content, LogType::WarningLog);
}

void Pt::Core::PMessageLogger::error(QString content) const
{
    log(content, LogType::ErrorLog);
}

void Pt::Core::PMessageLogger::fatal(QString content) const
{
    log(content, LogType::FatalLog);
}

void Pt::Core::PMessageLogger::displayLog(QString resultLog, LogType logType) const
{
    if (logType >= displayBound)
        QTextStream(stdout) << resultLog << endl;
}

void Pt::Core::PMessageLogger::writeLog(QString resultLog, LogType logType) const
{
    foreach (auto fileName, logFileNamePattern.value(logType)) {
        QFile file(fileName);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << resultLog << endl;
            file.close();
        }
    }
}

