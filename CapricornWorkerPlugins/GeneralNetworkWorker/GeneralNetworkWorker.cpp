#include "GeneralNetworkWorker.h"
#include <QThread>
#include <QDebug>
#include <QEventLoop>
#include "PLogger.h"

GeneralNetworkWorker::GeneralNetworkWorker()
{
    methods["download"] = ExecuteMethod(&GeneralNetworkWorker::download);
}

CapricornWorker::ResultType GeneralNetworkWorker::download(QJsonObject jobInfo)
{
    //PLogger logger;
    qDebug() << jobInfo["path"].toString();
    //text = jobInfo["path"].toString();
    qDebug() << "b";
    timeout = 10 * 1000;
    qDebug() << timeout;
    total = count = 0;
    QNetworkAccessManager manager;
    QJsonObject object = jobInfo["parameter"].toObject();
 //   m.get(QNetworkRequest(object["url"].toString()));
    qDebug() << timeout;
    QNetworkReply *reply = manager.get(QNetworkRequest(object["url"].toString()));
    qDebug() << "before";
    connect(reply, &QNetworkReply::readyRead, [=](){this->receivePayload(reply);});
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    qDebug() << "after";
    loop.exec();
    qDebug() << reply->readAll().size() << reply->error();
    ResultType result = Success;
    qDebug() << "finished";
    return result;
}

void GeneralNetworkWorker::receivePayload(QNetworkReply *reply)
{
    text += "ok";
    total += reply->readAll().size();
    count++;
    if (count % 1 == 0)
        qDebug() << count << ":" << total;// << text;
}


CapricornWorker *GeneralNetworkWorkerFactory::createInstance(Pt::Core::PLogger logger)
{
    qDebug() << "I know";
    return new GeneralNetworkWorker;
}
