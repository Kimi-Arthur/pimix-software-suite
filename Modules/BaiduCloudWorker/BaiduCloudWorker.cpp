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
#include "PChecksum.h"
#include <tuple>
#include "PLogger.h"

using namespace Pt::Core;
BaiduCloudWorker::BaiduCloudWorker(PLogger *_logger)
    : logger(_logger)
{
    logger->displayBound = PLogger::LogType::TraceLog;
    logger->logMethodIn("BaiduCloudWorker", "BaiduCloudWorker");
    manager->setRetryPolicy(PNetworkRetryPolicy::FixedIntervalRetryPolicy(600000, 5));
    QFile settingsFile("U:/BaiduCloud.json");
    if (!settingsFile.open(QIODevice::ReadOnly))
        return;
    auto value = settingsFile.readAll();
    settings = QJsonDocument::fromJson(value).object();
    qDebug() << settings;

    logger->logMethodOut("BaiduCloudWorker", "BaiduCloudWorker");
}

ResultType BaiduCloudWorker::downloadFile(QString remotePath, QString localPath)
{

}

ResultType BaiduCloudWorker::uploadFile(QString remotePath, QString localPath)
{
    logger->logMethodIn("BaiduCloudWorker", "uploadFile");

    logger->debug(remotePath, "remotePath");
    QFile f(localPath);
    if (!f.open(QIODevice::ReadOnly)) {
        logger->debug(PString::format("Local file \" {LocalPath}\" open error.", {{"LocalPath", localPath}}));
        logger->logMethodOut("BaiduCloudWorker", "uploadFile");
        return ResultType::Failure;
    }
    qint64 fileSize = f.size();
    f.close();
    // Try rapid upload first
    if (uploadFileRapid(remotePath, localPath) == ResultType::Success) {
        logger->debug(PString::format("Succeeded with uploadFileRapid, {LocalPath}", {{"LocalPath", localPath}}));
        logger->logMethodOut("BaiduCloudWorker", "uploadFile");
        return ResultType::Success;
    }
    if (fileSize <= BaseBlockSize)
        uploadFileDirect(remotePath, localPath);
    else uploadFileByBlockSinglethread(remotePath, localPath);

    logger->logMethodOut("BaiduCloudWorker", "uploadFile");
}

ResultType BaiduCloudWorker::removePath(QString remotePath)
{
    logger->logMethodIn("BaiduCloudWorker", "removePath");
    std::map<QString, QString> parameters = {
        {"RemotePath", remotePath},
        {"RemotePathPrefix", settings["RemotePathPrefix"].toString()},
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };

    manager->executeNetworkRequest(HttpVerb::Post, PString::format(settings["RemovePath"].toObject()["UrlPattern"].toString(), parameters));
    ResultType result = ResultType::Success;

    logger->logMethodOut("BaiduCloudWorker", "removePath");
    return result;
}

QStringList BaiduCloudWorker::getFileList()
{
    logger->logMethodIn("BaiduCloudWorker", "getFileList");
    std::map<QString, QString> parameters = {
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()},
        {"Cursor", "null"}
    };

    QNetworkReply *reply = manager->executeNetworkRequest(HttpVerb::Get, PString::format(settings["DiffFileList"].toObject()["UrlPattern"].toString(), parameters));

    QJsonObject diffFileListResult = QJsonDocument::fromJson(reply->readAll()).object();
    reply->deleteLater();
    QStringList result = diffFileListResult["entries"].toObject().keys();
    while (diffFileListResult["has_more"].toBool()) {
        parameters["Cursor"] = diffFileListResult["cursor"].toString();
        reply = manager->executeNetworkRequest(HttpVerb::Get, PString::format(settings["DiffFileList"].toObject()["UrlPattern"].toString(), parameters));
        diffFileListResult = QJsonDocument::fromJson(reply->readAll()).object();
        reply->deleteLater();
        result.append(diffFileListResult["entries"].toObject().keys());
    }
    result.replaceInStrings(QRegularExpression("^/apps/Pimix/"), "");
    logger->debug(result, "File List");
    logger->logMethodOut("BaiduCloudWorker", "getFileList");
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

std::map<QString, QString> BaiduCloudWorker::getFileInfos(const QString &localPath)
{
    logger->logMethodIn("BaiduCloudWorker", "getFileInfos");
    std::map<QString, QString> result;
    QFile f(localPath);
    if (f.open(QIODevice::ReadOnly)) {
        qint64 fileSize = f.size();
        result["ContentLength"] = QString::number(fileSize);
        result["SliceMd5"] = QCryptographicHash::hash(f.read(256 * PFile::KilobyteSize), QCryptographicHash::Md5).toHex();
        f.seek(0);
        QCryptographicHash md5(QCryptographicHash::Md5);
        PChecksum crc(PChecksum::Algorithm::Crc32);
        QByteArray block = f.read(BaseBlockSize);
        while (!block.isEmpty()) {
            md5.addData(block);
            crc.addData(block);
            block = f.read(BaseBlockSize);
        }
        result["ContentMd5"] = md5.result().toHex();
        result["ContentCrc32"] = crc.result().toHex();
    }
    logger->debug(result["ContentMd5"]);
    logger->debug(result["ContentCrc32"]);
    logger->logMethodOut("BaiduCloudWorker", "getFileInfos");
    return result;
}

ResultType BaiduCloudWorker::uploadFileRapid(const QString &remotePath, const QString &localPath)
{
    std::map<QString, QString> parameters = {
        {"RemotePath", remotePath},
        {"RemotePathPrefix", settings["RemotePathPrefix"].toString()},
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };
    std::map<QString, QString> fileInfos = getFileInfos(localPath);
    parameters.insert(fileInfos.begin(), fileInfos.end());
    foreach (auto item, parameters)
        qDebug() << item.first << item.second;
    auto reply = manager->executeNetworkRequest(HttpVerb::Post, PString::format(settings["UploadFileRapid"].toObject()["UrlPattern"].toString(), parameters));

    return (reply->error() == QNetworkReply::NoError) ? ResultType::Success : ResultType::Failure;
}

ResultType BaiduCloudWorker::uploadFileDirect(QString remotePath, QString localPath)
{
    logger->logMethodIn("BaiduCloudWorker", "uploadFileDirect");
    std::map<QString, QString> parameters = {
        {"RemotePath", remotePath},
        {"RemotePathPrefix", settings["RemotePathPrefix"].toString()},
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };
    QFile fileToUpload(localPath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return ResultType::Failure;
    manager->executeNetworkRequest(HttpVerb::Put, PString::format(settings["UploadFileDirect"].toObject()["UrlPattern"].toString(), parameters), fileToUpload.readAll());

    ResultType result = ResultType::Success;
    logger->debug(result, "result");
    logger->logMethodOut("BaiduCloudWorker", "uploadFileDirect");
    return result;
}

ResultType BaiduCloudWorker::uploadFileByBlockMultithread(QString remotePath, QString localPath)
{
    QFile fileToUpload(localPath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return ResultType::Failure;
    qint64 fileSize = fileToUpload.size();
    //fileToUpload.close();
    QList<QPair<qint64, qint64>> blockInformationList;
    for (qint64 index = 0; index < fileSize; index += BaseBlockSize) {
        blockInformationList.append({index, qMin(BaseBlockSize, fileSize - index)});
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

ResultType BaiduCloudWorker::uploadFileByBlockSinglethread(QString remotePath, QString localPath)
{
    QFile fileToUpload(localPath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return ResultType::Failure;
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

bool BaiduCloudWorker::verifyFile(const QString &remotePath)
{
    std::map<QString, QString> parameters = {
        {"RemotePath", remotePath},
        {"RemotePathPrefix", settings["RemotePathPrefix"].toString()},
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };

    manager->executeNetworkRequest(HttpVerb::Post, PString::format(settings["VerifyFile"].toObject()["UrlPattern"].toString(), parameters));
    return false;
}

QString BaiduCloudWorker::uploadBlock(const QByteArray &data)
{
    qDebug() << "UploadBlock" << data.size();
    std::map<QString, QString> parameters = {
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };
    PNetworkAccessManager m;
    m.setRetryPolicy(PNetworkRetryPolicy::FixedIntervalRetryPolicy(6000000, 5));
    QNetworkReply *reply = m.executeNetworkRequest(HttpVerb::Put, PString::format(settings["UploadBlock"].toObject()["UrlPattern"].toString(), parameters), data);
    QJsonObject blockUploadResult = QJsonDocument::fromJson(reply->readAll()).object();
    reply->deleteLater();
    qDebug() << "result" << blockUploadResult;
    return blockUploadResult["md5"].toString();
}

ResultType BaiduCloudWorker::mergeBlocks(QString remotePath, QStringList blockHashList)
{
    std::map<QString, QString> parameters = {
        {"RemotePath", remotePath},
        {"RemotePathPrefix", settings["RemotePathPrefix"].toString()},
        {"AccessToken", settings["Accounts"].toObject()["PimixT"].toObject()["AccessToken"].toString()}
    };
    QJsonObject param;
    param["block_list"] = QJsonArray::fromStringList(blockHashList);
    manager->executeNetworkRequest(HttpVerb::Post, PString::format(settings["MergeBlocks"].toObject()["UrlPattern"].toString(), parameters), QByteArray("param=") + QJsonDocument(param).toJson());

    ResultType result = ResultType::Success;
    qDebug() << "finished";
    return result;
}
