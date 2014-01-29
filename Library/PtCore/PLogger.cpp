#include "PLogger.h"
#include <QTextStream>
#include <QFile>
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
        logFileNamePattern.insert(t, {"{LogFileNamePattern}.log", "{LogFileNamePattern}-debug.log",
                                      "{LogFileNamePattern}-full.log"});

    logFileNamePattern.insert(LogType::DebugLog, {"{LogFileNamePattern}-debug.log", "{LogFileNamePattern}-full.log"});

    logFileNamePattern.insert(LogType::TraceLog, {"{LogFileNamePattern}-full.log"});
    displayBound = DefaultDisplayBound;
}

void Pt::Core::PLogger::displayLog(QString resultLog, LogType logType) const
{
    if (logType >= displayBound)
        QTextStream(stdout) << resultLog << endl;
}

void Pt::Core::PLogger::writeLog(QString resultLog, LogType logType) const
{
    auto parameters = globalParamters;
    parameters.append(PStringPairList {
        {"date", QDate::currentDate().toString(Qt::ISODate)}
    });

    foreach (auto fileName, logFileNamePattern.value(logType)) {
        QFile file(PString::format(fileName, parameters));
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            stream << resultLog << endl;
            file.close();
        }
    }
}
