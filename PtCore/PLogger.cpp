#include "PLogger.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QStringList>

Pt::Core::PLogger *Pt::Core::PLogger::staticInstance = nullptr;

Pt::Core::PLogger *Pt::Core::PLogger::globalInstance()
{
    if (staticInstance == nullptr)
        staticInstance = new PLogger();
    return staticInstance;
}

Pt::Core::PLogger::PLogger()
{
    logPattern = DefaultLogPattern;
    for (LogType t = LogType::InformationLog; t <= LogType::FatalLog; t = static_cast<LogType>(static_cast<int>(t) + 1))
        logFileNamePattern.insert(t, {DefaultLogFileNamePattern + ".log"});
    logFileNamePattern.insert(LogType::DebugLog, {DefaultLogFileNamePattern + "-debug.log"});
    logFileNamePattern.insert(LogType::DebugLog, {DefaultLogFileNamePattern + "-full.log"});
    logFileNamePattern.insert(LogType::DebugLog, {DefaultLogFileNamePattern + "-full.log"});
    displayBound = DefaultDisplayBound;
}

void Pt::Core::PLogger::displayLog(QString resultLog, LogType logType) const
{
    if (logType >= displayBound)
        QTextStream(stdout) << resultLog << endl;
}

void Pt::Core::PLogger::writeLog(QString resultLog, LogType logType) const
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
