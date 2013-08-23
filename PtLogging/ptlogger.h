#ifndef PTLOGGER_H
#define PTLOGGER_H

#include "ptlogging_global.h"
#include <QStringList>
#include <QDateTime>
#include <QTextStream>
#include <QFile>

class PTLOGGINGSHARED_EXPORT PtLogger
{
    QString logPattern;
    QTextStream outputStream;
    QFile *outputFile;
public:
    PtLogger();

    void log(QStringList content = {}, QString type="INFO");
    // Setter/Getters
    QString getLogPattern() const;
    void setLogPattern(const QString &value);
    void setOutputStream(const QString &value);
};

#endif // PTLOGGER_H
