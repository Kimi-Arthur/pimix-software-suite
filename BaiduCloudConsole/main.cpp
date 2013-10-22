#include <QCoreApplication>
#include "PConsole.h"
#include "BaiduCloudWorker.h"
#include "PLogger.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << Pt::Core::PSerializer::serialize(qint64(16));
    qDebug() << Pt::Core::PSerializer::serialize(QString("123"));
//    Pt::Core::PLogger::globalInstance()->logPattern = "abc";
//    auto parameters = QCoreApplication::arguments();
//    BaiduCloudWorker w;
//    w.uploadFile(parameters[1], parameters[2]);
}
