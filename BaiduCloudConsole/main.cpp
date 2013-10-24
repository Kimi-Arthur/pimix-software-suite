#include <QCoreApplication>
#include "PConsole.h"
#include "BaiduCloudWorker.h"
#include "PLogger.h"

#include <QDebug>

using namespace Pt::Core;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << PSerializer::serialize(qint64(16));
    qDebug() << PSerializer::serialize<char *>("123");
    PLogger::globalInstance()->error("是厉害啊！");
//    qDebug() << PSerializer::deserialize<char *>("abc");
//    Pt::Core::PLogger::globalInstance()->logPattern = "abc";
//    auto parameters = QCoreApplication::arguments();
//    BaiduCloudWorker w;
//    w.uploadFile(parameters[1], parameters[2]);
}
