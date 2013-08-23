#include "ptlogger.h"

PtLogger::PtLogger(QObject *parent) :
    QObject(parent)
{
    setLogPattern("[{time}][{type}]{content}\n");
}

void PtLogger::log(QStringList content, QString type)
{
    QList<QPair<QString, QString>> parameters = {
        {"time", QDateTime::currentDateTime().toString(Qt::ISODate)},
        {"type", type},
        {"content", content.join(" ")}
    };
    QString result = logPattern();
    foreach (auto parameter, parameters)
        result.replace("{" + parameter.first + "}", parameter.second);
    outputStream()->wr

            QTextStream *PtLogger::outputFile() const
    {
        return m_outputStream;
    }

    void PtLogger::setOutputFile(QTextStream *outputFile)
    {
        m_outputStream = outputFile;
    }
}
