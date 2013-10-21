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

//Pt::Core::PLogger::PLogger(QPluginLoader &loader) :
//    PLogger()
//{

//}


void Pt::Core::PLogger::log(QString content, LogType logType) const
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

void Pt::Core::PLogger::debug(QString content) const
{
    log(content, LogType::DebugLog);
}

void Pt::Core::PLogger::info(QString content) const
{
    log(content, LogType::InformationLog);
}

void Pt::Core::PLogger::warn(QString content) const
{
    log(content, LogType::WarningLog);
}

void Pt::Core::PLogger::error(QString content) const
{
    log(content, LogType::ErrorLog);
}

void Pt::Core::PLogger::fatal(QString content) const
{
    log(content, LogType::FatalLog);
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


template<class T>
QString Pt::Core::PLogger::objectToString(const QList<T> &objectList)
{
    QStringList stringList;
    foreach (auto object, objectList)
        stringList.append(objectToString(object));
    return QString("[%1]").arg(stringList.join(ListSeparator));
}


void Pt::Core::PLogWriter::writeLog(const QStringList &contentList)
{
    foreach (auto ts, outputStreamList) {
        foreach (auto &content, contentList) {
            *ts << content << endl;
        }
    }
}

QStringList Pt::Core::PLogWriter::toObjectString(const QString &raw)
{
    return QStringList(QStringLiteral("\"%1\"").arg(raw));
}

template<class T>
QStringList Pt::Core::PLogWriter::toObjectString(const QList<T> &raw)
{
    QStringList resultList;
    foreach (T &item, raw)
        resultList.append(toObjectString(raw));
    return "[" + resultList.join(", ") + "]";
}

/*void Pt::Core::PLogWriter::log(QString content)
{
    writeLog(content);
}*/

template<class T>
void Pt::Core::PLogWriter::log(T object, const QString &objectName)
{
    writeLog(objectName + "=" + toObjectString(object));
}

/*
template<class T>
void Pt::Core::PLogWriter::log(T object, Pt::Core::PLogWriter::ToObjectStringFunction toObjectStringFunction, const QString &objectName)
{
    writeLog(objectName + "=" + toObjectStringFunction(object));
}
*/
