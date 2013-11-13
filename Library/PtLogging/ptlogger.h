#ifndef PTLOGGER_H
#define PTLOGGER_H

#include "ptlogging_global.h"
#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QTextStream>
#include <QFile>

class PTLOGGINGSHARED_EXPORT PtLogger : public QObject
{
    Q_OBJECT
public:
    explicit PtLogger(QObject *parent = 0);
    Q_PROPERTY(QString outputFileName READ outputFileName WRITE setOutputFileName NOTIFY outputFileNameChanged)
    Q_PROPERTY(QString logPattern READ logPattern WRITE setLogPattern NOTIFY logPatternChanged)
    //void log(QStringList content = {}, QString type="INFO");
private:
    Q_PROPERTY(QTextStream *outputStream READ outputStream WRITE setOutputStream RESET resetOutputStream NOTIFY outputStreamChanged)

public:
    QString outputFileName() const
    {
        return m_outputFileName;
    }

    QString logPattern() const
    {
        return m_logPattern;
    }

    QTextStream * outputStream()
    {
        if (m_outputStream == nullptr) {
            // stdout not enabled
            QFile *file = new QFile(outputFileName());
            if (file->open(QIODevice::Append))
                m_outputStream = new QTextStream(file);
            else return nullptr;

        }
        return m_outputStream;
    }

private:
    QString m_outputFileName;

    QString m_logPattern;

    QTextStream * m_outputStream;

signals:

void outputFileNameChanged(QString arg);

void logPatternChanged(QString arg);

void outputStreamChanged(QFile * arg);

public slots:

void setOutputFileName(QString arg)
{
    if (m_outputFileName != arg) {
        m_outputFileName = arg;
        resetOutputStream();
        emit outputFileNameChanged(arg);
    }
}
void setLogPattern(QString arg)
{
    if (m_logPattern != arg) {
        m_logPattern = arg;
        emit logPatternChanged(arg);
    }
}
void setOutputStream(QFile * arg)
{
    if (m_outputStream != arg) {
        m_outputStream->device()->close();
        m_outputStream = arg;
        emit outputStreamChanged(arg);
    }
}

void resetOutputStream()
{
    if (m_outputStream != nullptr) {
        m_outputStream->close();
        m_outputStream->deleteLater();
        m_outputStream = nullptr;
        emit outputStreamChanged(nullptr);
    }
}
};

#endif // PTLOGGER_H
