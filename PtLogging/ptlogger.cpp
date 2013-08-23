#include "ptlogger.h"

PtLogger::PtLogger()
{
    setLogPattern("[{time}][{type}]{content}\n");
    outputFile->open(stdout, QIODevice::Append);
    outputStream.setDevice(outputFile);
}

void PtLogger::log(QStringList content, QString type)
{
    QList<QPair<QString, QString>> parameters = {
        {"time", QDateTime::currentDateTime().toString(Qt::ISODate)},
        {"type", type},
        {"content", content.join(" ")}
    };
    QString result = getLogPattern();
    foreach (auto parameter, parameters)
        result.replace("{" + parameter.first + "}", parameter.second);
    outputStream << result << endl;
}

void PtLogger::setOutputStream(const QString &value)
{
    outputFile->close();
    outputFile->deleteLater();
    outputFile = new QFile(value);
    if (!outputFile->open(QIODevice::Append)) // Value.isEmpty() is implied
        outputFile->open(stdout, QIODevice::Append);
    outputStream.setDevice(outputFile);
}


QString PtLogger::getLogPattern() const
{
    return logPattern;
}

void PtLogger::setLogPattern(const QString &value)
{
    logPattern = value;
}
