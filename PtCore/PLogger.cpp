#include "PLogger.h"
#include <QTextStream>
#include <QFile>

PLogger::PLogger()
{
    logPattern = DefaultLogPattern;
}

void PLogger::log(QString content, PLogger::LogType logType) const
{
    QList<QPair<QString, QString>> parameters = {
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

void PLogger::debug(QString content) const
{
    log(content, DebugLog);
}

void PLogger::info(QString content) const
{
    log(content, InformationLog);
}

void PLogger::warn(QString content) const
{
    log(content, WarningLog);
}

void PLogger::error(QString content) const
{
    log(content, ErrorLog);
}

void PLogger::fatal(QString content) const
{
    log(content, FatalLog);
}

void PLogger::displayLog(QString resultLog, PLogger::LogType logType) const
{
    if (logType >= displayBound)
        QTextStream(stdout) << resultLog << endl;
}

void PLogger::writeLog(QString resultLog, PLogger::LogType logType) const
{
    foreach (auto fileName, logFileMapping.value(logType)) {
        QFile file(fileName);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << resultLog << endl;
            file.close();
        }
    }
}

