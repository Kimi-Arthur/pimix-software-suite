#include "GeneralNetworkWorker.h"
#include <QThread>
#include <QDebug>
#include <QEventLoop>

QString GeneralNetworkWorker::text = "abc";

GeneralNetworkWorker::GeneralNetworkWorker()
{
    methods["download"] = ExecuteMethod(&GeneralNetworkWorker::download);
}

CapricornWorker::ResultType GeneralNetworkWorker::download(QJsonObject jobInfo)
{
    timeout = 10 * 1000;
    total = count = 0;
    QNetworkAccessManager manager;
    QJsonObject object = jobInfo["parameter"].toObject();
    QNetworkReply *reply = manager.get(QNetworkRequest(object["url"].toString()));
    qDebug() << "before";
    connect(reply, &QNetworkReply::readyRead, [=](){this->receivePayload(reply);});
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    qDebug() << "after";
    loop.exec();
    qDebug() << reply->readAll().size();
    ResultType result = Success;
    qDebug() << "finished";
    return result;
}

void GeneralNetworkWorker::receivePayload(QNetworkReply *reply)
{
    total += reply->readAll().size();
    count++;
    qDebug() << count << ":" << total;
}
