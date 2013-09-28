#include "BaiduCloudWorker.h"
#include <QEventLoop>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtMath>
#include <QJsonArray>
#include <QTimer>

BaiduCloudWorker::BaiduCloudWorker()
{
    QFile settingsFile("U:/BaiduCloud.json");
    if (!settingsFile.open(QIODevice::ReadOnly))
        return;
    auto value = settingsFile.readAll();
    settings = QJsonDocument::fromJson(value).object();
    qDebug() << settings;
    //methods["download"] = ExecuteMethod(&GeneralNetworkWorker::download);
}

CapricornWorker::ResultType BaiduCloudWorker::downloadFile(QString remotePath, QString localPath)
{

}

CapricornWorker::ResultType BaiduCloudWorker::uploadFile(QString remotePath, QString localPath)
{
    qDebug() << "First";
    QFile f(localPath);
    if (!f.open(QIODevice::ReadOnly))
        return Failure;
    qint64 fileSize = f.size();
    f.close();
    if (fileSize <= baseBlockSize)
        uploadFileDirect(remotePath, localPath);
    else uploadFileByBlock(remotePath, localPath);
}

CapricornWorker::ResultType BaiduCloudWorker::removePath(QString remotePath)
{
    std::map<QString, QString> parameters = {
        {"RemotePath", remotePath},
        {"RemotePathPrefix", settings["RemotePathPrefix"].toString()},
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };

    executeHttpRequest(Pt::HttpVerb::Post, settings["RemovePath"].toObject()["UrlPattern"].toString(), parameters);
    ResultType result = Success;
    qDebug() << "finished";
    return result;
}

void BaiduCloudWorker::showProgress(qint64 bs, qint64 bt)
{
    if (bs - bc > 500 * PFile::KilobyteSize) {
        qDebug() << "Progress" << bs << bt;
        bc = bs;
    }
}

qint64 BaiduCloudWorker::getBlockSize(qint64 fileSize)
{
    if (fileSize > maxBlockSize * maxBlockCount)
        return 0;
}

CapricornWorker::ResultType BaiduCloudWorker::uploadFileDirect(QString remotePath, QString localPath)
{
    std::map<QString, QString> parameters = {
        {"RemotePath", remotePath},
        {"RemotePathPrefix", settings["RemotePathPrefix"].toString()},
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };
    QFile fileToUpload(localPath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return Failure;
    executeHttpRequest(Pt::HttpVerb::Put, settings["UploadFileDirect"].toObject()["UrlPattern"].toString(), parameters, fileToUpload.readAll());

    ResultType result = Success;
    qDebug() << "finished";
    return result;
}

CapricornWorker::ResultType BaiduCloudWorker::uploadFileByBlock(QString remotePath, QString localPath)
{
    QFile fileToUpload(localPath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return Failure;
    QStringList blockHashList;
    QByteArray data = fileToUpload.read(baseBlockSize);
    qDebug() << "Start";
    while (!data.isEmpty()) {
        bc = 0;
        blockHashList.append(uploadBlock(data));
        //executeHttpRequest(Pt::HttpVerb::Get, "http://www.baidu.com");
        qDebug() << blockHashList << blockHashList.count();
        data = fileToUpload.read(baseBlockSize);
    }
    return mergeBlocks(remotePath, blockHashList);
}

QString BaiduCloudWorker::uploadBlock(const QByteArray &data)
{
    std::map<QString, QString> parameters = {
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };
    QJsonObject blockUploadResult = QJsonDocument::fromJson(executeHttpRequest(Pt::HttpVerb::Put,
                                                                               settings["UploadBlock"].toObject()["UrlPattern"].toString(), parameters, data)).object();
    return blockUploadResult["md5"].toString();
}

CapricornWorker::ResultType BaiduCloudWorker::mergeBlocks(QString remotePath, QStringList blockHashList)
{
    std::map<QString, QString> parameters = {
        {"RemotePath", remotePath},
        {"RemotePathPrefix", settings["RemotePathPrefix"].toString()},
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };
    QJsonObject param;
    param["block_list"] = QJsonArray::fromStringList(blockHashList);
    executeHttpRequest(Pt::HttpVerb::Post, settings["MergeBlocks"].toObject()["UrlPattern"].toString(), parameters , QByteArray("param=") + QJsonDocument(param).toJson());

    ResultType result = Success;
    qDebug() << "finished";
    return result;
}

QByteArray BaiduCloudWorker::executeHttpRequest(Pt::HttpVerb verb, QString urlPattern, std::map<QString, QString> parameters, const QByteArray &data)
{
    for (auto parameter : parameters)
        urlPattern.replace(QString("{%1}").arg(parameter.first), parameter.second);
    QNetworkReply *reply;
    switch (verb) {
    case Pt::HttpVerb::Get:
        reply = manager->get(QNetworkRequest(urlPattern));
        break;
    case Pt::HttpVerb::Post:
        reply = manager->post(QNetworkRequest(urlPattern), data);
        break;
    case Pt::HttpVerb::Put:
        reply = manager->put(QNetworkRequest(urlPattern), data);
        break;
    case Pt::HttpVerb::Head:
    case Pt::HttpVerb::Delete:
    case Pt::HttpVerb::Trace:
    case Pt::HttpVerb::Options:
    case Pt::HttpVerb::Connect:
    case Pt::HttpVerb::Patch:
    default:
        break;
    }
    int i = 0;
    forever {
        qDebug() << QString("Tried %1 times").arg(i++);
        QEventLoop loop;
        QTimer timer;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        connect(reply, &QNetworkReply::uploadProgress, this, &BaiduCloudWorker::showProgress);
        connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
        timer.start(600000);
        loop.exec();
        if (timer.isActive())
            break;
    }
    reply->deleteLater();
    return reply->readAll();
}

