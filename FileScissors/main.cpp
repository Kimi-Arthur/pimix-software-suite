#include <QCoreApplication>
#include "PFile.h"
#include "PConsole.h"
#include <QDebug>
#include <QDateTime>
#include <QMap>


int main(int argc, char *argv[])
{
    QStringList parameters(PConsole::extractParameters(argc, argv));

    //QMap<QString, PConsole::ConsoleExecutionMethod> mapping = {{"test", [](QStringList l) {qDebug() << l.size();}}};
    //mapping["test"]({"a", "b", "c"});

    qDebug() << QDateTime::currentDateTime();
    int count = PFile::splitFile(argv[1], argv[2], QString(argv[3]).toLongLong());
    qDebug() << QDateTime::currentDateTime();
    QStringList l;
    l.reserve(count);
    for (int i = 0; i < count; i++)
        l.append(QString(argv[2]) + ".p" + QString("%1").arg(i, 2, 10, QChar('0')));
    PFile::mergeFiles(l, QString(argv[2]) + ".test");
    qDebug() << QDateTime::currentDateTime();
    return 0;
}
