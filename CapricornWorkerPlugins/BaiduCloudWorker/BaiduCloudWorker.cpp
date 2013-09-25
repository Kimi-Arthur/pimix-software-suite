#include "BaiduCloudWorker.h"
#include <QEventLoop>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtMath>
#include <QJsonArray>

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
    executeHttpRequest(Pt::HttpVerb::Put, settings["UploadFileDirect"].toObject()["UrlPattern"].toString(), parameters, new QByteArray(fileToUpload.readAll()));

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
    QByteArray *data = new QByteArray(fileToUpload.read(baseBlockSize));
    qDebug() << "Start";
    while (!data->isEmpty()) {
        blockHashList.append(uploadBlock(data));
        qDebug() << blockHashList;
        delete data;
        data = new QByteArray(fileToUpload.read(baseBlockSize));
    }
    return mergeBlocks(remotePath, blockHashList);
}

QString BaiduCloudWorker::uploadBlock(QByteArray *data)
{
    std::map<QString, QString> parameters = {
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };
    QJsonObject blockUploadResult = QJsonDocument::fromJson(executeHttpRequest(Pt::HttpVerb::Put,
                                                                               settings["UploadBlock"].toObject()["UrlPattern"].toString(),
                                                                               parameters, data)).object();
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
    executeHttpRequest(Pt::HttpVerb::Post, settings["MergeBlocks"].toObject()["UrlPattern"].toString(), parameters , new QByteArray(QByteArray("param=") + QJsonDocument(param).toJson()));

    ResultType result = Success;
    qDebug() << "finished";
    return result;
}

QByteArray BaiduCloudWorker::executeHttpRequest(Pt::HttpVerb verb, QString urlPattern, std::map<QString, QString> parameters, QByteArray *data)
{
    for (auto parameter : parameters)
        urlPattern.replace(QString("{%1}").arg(parameter.first), parameter.second);
    auto manager = new QNetworkAccessManager();
    QNetworkReply *reply;
    switch (verb) {
    case Pt::HttpVerb::Get:
        reply = manager->get(QNetworkRequest(urlPattern));
        break;
    case Pt::HttpVerb::Post:
        reply = manager->post(QNetworkRequest(urlPattern), *data);
        break;
    case Pt::HttpVerb::Put:
        reply = manager->put(QNetworkRequest(urlPattern), *data);
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
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    return reply->readAll();
}

