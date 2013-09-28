#include <QCoreApplication>
#include "PConsole.h"
#include "BaiduCloudWorker.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    auto parameters = PConsole::extractParameters(argc, argv);
    BaiduCloudWorker w;
    w.uploadFile(parameters[1], parameters[2]);
}
