#include "GeneralNetworkWorker.h"
#include <QThread>
#include <QDebug>

QString GeneralNetworkWorker::text = "abc";

GeneralNetworkWorker::GeneralNetworkWorker()
{
    methods["download"] = static_cast<ExecuteMethod>(&GeneralNetworkWorker::download);
}

CapricornWorker::ResultType GeneralNetworkWorker::download(QJsonObject jobInfo)
{
    text += "abc";
    while(1) {
        qDebug() << jobInfo << text;
        QThread::sleep(10);
    }
    return Success;
}
