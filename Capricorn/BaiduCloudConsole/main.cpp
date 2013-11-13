#include <QCoreApplication>
#include "PConsole.h"
#include "BaiduCloudWorker.h"
#include "PLogger.h"
#include "PJsonValue.h"
#include <QDebug>

using namespace Pt::Core;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    BaiduCloudWorker w;
    w.uploadFile("ttest/as.mkv", "U:/1stHalf.mkv");
//    qDebug() << PSerializer::serialize(qint64(16));
//    QJsonObject o;
//    o.insert("1", QJsonValue(QString("2")));
//    PJsonValue v = PJsonValue(o);
//    qDebug() << PSerializer::serialize(v);
//    qDebug() << PSerializer::deserialize<char *>("是么");
//    PLogger::globalInstance()->error("是厉害啊！");
//    qDebug() << PSerializer::deserialize<char *>("abc");
//    Pt::Core::PLogger::globalInstance()->logPattern = "abc";
//    auto parameters = QCoreApplication::arguments();
//    BaiduCloudWorker w;
//    w.uploadFile(parameters[1], parameters[2]);
}
