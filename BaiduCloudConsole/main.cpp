#include <QCoreApplication>
#include "PConsole.h"
#include "BaiduCloudWorker.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto parameters = QCoreApplication::arguments();
    BaiduCloudWorker w;
    w.uploadFile(parameters[1], parameters[2]);
}
