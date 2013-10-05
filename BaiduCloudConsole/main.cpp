#include <QCoreApplication>
#include "PConsole.h"
#include "BaiduCloudWorker.h"
#include "PMessageLogger.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Pt::Core::PMessageLogger::globalInstance()->logPattern = "abc";
    auto parameters = QCoreApplication::arguments();
    BaiduCloudWorker w;
    w.uploadFile(parameters[1], parameters[2]);
}
