#include "BaiduCloudWorker.h"
#include <QEventLoop>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtMath>
#include <QJsonArray>
#include <QPair>
#include <QTimer>
#include <QStringList>
#include <QtConcurrent/QtConcurrent>
#include "PLogger.h"
#include <tuple>
#include "PLogger.h"

BaiduCloudWorker::BaiduCloudWorker(PLogger *_logger)
    : logger(_logger)
{
    logger->trace("[MethodIn ] BaiduCloudWorker::BaiduCloudWorker");
    manager->setRetryPolicy(PNetworkRetryPolicy::FixedIntervalRetryPolicy(600000, 5));
    QFile settingsFile("U:/BaiduCloud.json");
    if (!settingsFile.open(QIODevice::ReadOnly))
        return;
    auto value = settingsFile.readAll();
    settings = QJsonDocument::fromJson(value).object();
    qDebug() << settings;
    logger->trace("[MethodOut] BaiduCloudWorker::BaiduCloudWorker");
}

CapricornWorker::ResultType BaiduCloudWorker::downloadFile(QString remotePath, QString localPath)
{

}

CapricornWorker::ResultType BaiduCloudWorker::uploadFile(QString remotePath, QString localPath)
{
    logger->trace("[MethodIn ] BaiduCloudWorker::uploadFile");
    QFile f(localPath);
    if (!f.open(QIODevice::ReadOnly))
        return Failure;
    qint64 fileSize = f.size();
    f.close();
    if (fileSize <= BaseBlockSize)
        uploadFileDirect(remotePath, localPath);
    else uploadFileByBlockSinglethread(remotePath, localPath);
    logger->trace("[MethodOut] BaiduCloudWorker::uploadFile");
}

CapricornWorker::ResultType BaiduCloudWorker::removePath(QString remotePath)
{
    std::map<QString, QString> parameters = {
        {"RemotePath", remotePath},
        {"RemotePathPrefix", settings["RemotePathPrefix"].toString()},
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };

    manager->executeNetworkRequest(HttpVerb::Post, settings["RemovePath"].toObject()["UrlPattern"].toString(), parameters);
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
    if (fileSize > MaxBlockSize * MaxBlockCount)
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
    manager->executeNetworkRequest(HttpVerb::Put, settings["UploadFileDirect"].toObject()["UrlPattern"].toString(), parameters, fileToUpload.readAll());

    ResultType result = Success;
    qDebug() << "finished";
    return result;
}

CapricornWorker::ResultType BaiduCloudWorker::uploadFileByBlockMultithread(QString remotePath, QString localPath)
{
    QFile fileToUpload(localPath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return Failure;
    qint64 fileSize = fileToUpload.size();
    //fileToUpload.close();
    QList<QPair<qint64, qint64>> blockInformationList;
    for (qint64 index = 0; index < fileSize; index += BaseBlockSize) {
        // Temp workaround for MinGW bug
        qint64 sizeA = BaseBlockSize, sizeB = fileSize - index;
        blockInformationList.append({index, qMin(sizeA, sizeB)});
    }

    //QtConcurrent::blockingMapped(blockInformationList,
    /**[&](QPair<qint64, qint64> bi)
    {
        QFile f;//(localPath);
        f.open(QIODevice::ReadOnly);
        f.seek(bi.first);
        QByteArray data = f.read(bi.second);
        f.close();
        //return uploadBlock(data);
    });*/

    //QStringList blockHashList;

    QByteArray data = fileToUpload.read(BaseBlockSize);
    qDebug() << QStringList("Start");
    bool ok = true;
    // QList needn't be thread safe here.
    // All operation to QList is done here in the same thread.
    QList<QFuture<QString>> blockStatusList;
    QList<QString> blockResultList;
    while (!data.isEmpty()) {
        bc = 0;
        blockStatusList.append(QtConcurrent::run(this, &BaiduCloudWorker::uploadBlock, data));
        while (blockStatusList.size() > MaxThreadCount) {
            QFuture<QString> currentBlockStatus = blockStatusList.takeFirst();
            currentBlockStatus.waitForFinished();
            blockResultList.append(currentBlockStatus.result());
            //Pt::Core::PMessageLogger::info(blockResultList);
        }
        qDebug() << "WIP" << blockStatusList.size() << "Done" << blockResultList.size();
//        ok = ok && !blockHashList.back().isEmpty();
        qDebug() << QDateTime::currentDateTime();
//        qDebug() << blockHashList.count() << ok;
        data = fileToUpload.read(BaseBlockSize);
    }
    while (!blockStatusList.isEmpty()) {
        QFuture<QString> currentBlockStatus = blockStatusList.takeFirst();
        currentBlockStatus.waitForFinished();
        blockResultList.append(currentBlockStatus.result());
        qDebug() << blockResultList;
    }
    return mergeBlocks(remotePath, blockResultList);
}

CapricornWorker::ResultType BaiduCloudWorker::uploadFileByBlockSinglethread(QString remotePath, QString localPath)
{
    QFile fileToUpload(localPath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return Failure;
    QStringList blockHashList;

    QByteArray data = fileToUpload.read(BaseBlockSize);
    qDebug() << "Start";
    bool ok = true;
    while (!data.isEmpty()) {
        bc = 0;
        blockHashList.append(uploadBlock(data));
        ok = ok && !blockHashList.back().isEmpty();
        qDebug() << QDateTime::currentDateTime();
        qDebug() << blockHashList.count() << ok;
        qDebug() << blockHashList;
        data = fileToUpload.read(BaseBlockSize);
    }
    return mergeBlocks(remotePath, blockHashList);
}

QString BaiduCloudWorker::uploadBlock(const QByteArray &data)
{
    qDebug() << "UploadBlock" << data.size();
    std::map<QString, QString> parameters = {
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };
    PNetworkAccessManager m;
    m.setRetryPolicy(PNetworkRetryPolicy::FixedIntervalRetryPolicy(6000000, 5));
    QNetworkReply *reply = m.executeNetworkRequest(HttpVerb::Put,
                                                          settings["UploadBlock"].toObject()["UrlPattern"].toString(), parameters, data);
    QJsonObject blockUploadResult = QJsonDocument::fromJson(reply->readAll()).object();
    reply->deleteLater();
    qDebug() << "result" << blockUploadResult;
    return blockUploadResult["md5"].toString();
}

QString BaiduCloudWorker::uploadBlockFromFile(const QString &filePath, qint64 startIndex, qint64 length)
{
    QFile fileToUpload(filePath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return "";
    fileToUpload.seek(startIndex);
    return uploadBlock(fileToUpload.read(length));
}

QString BaiduCloudWorker::uploadBlockFromMyFile(const std::tuple<QString, qint64, qint64> &parameters)
{
    return uploadBlockFromFile(std::get<0>(parameters), std::get<1>(parameters), std::get<2>(parameters));
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
    manager->executeNetworkRequest(HttpVerb::Post, settings["MergeBlocks"].toObject()["UrlPattern"].toString(), parameters , QByteArray("param=") + QJsonDocument(param).toJson());

    ResultType result = Success;
    qDebug() << "finished";
    return result;
}
