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
    logger->logMethodIn(__PFUNC_ID__);
    manager->setRetryPolicy(PNetworkRetryPolicy::LimitedRetryPolicy(600000, 5));
    QFile settingsFile(settingsFileName);
    if (!settingsFile.open(QIODevice::ReadOnly)) {
        logger->debug(PString::format("Settings file {SettingsFile} not found!", {{"SettingsFile", settingsFileName}}));
        logger->logMethodOut(__PFUNC_ID__);
        return;
    }
    auto value = settingsFile.readAll();
    settings = PJsonValue(QJsonDocument::fromJson(value).object());
    settingsMap = settings.toMap();
    qDebug() << settings;
    for (auto kv : settings.toMap())
        qDebug() << kv.first << kv.second;

    logger->logMethodOut(__PFUNC_ID__);
}

ResultType BaiduCloudWorker::downloadFile(QString remotePath, QString localPath)
{
    return ResultType::Failure;
}

ResultType BaiduCloudWorker::uploadFile(QString remotePath, QString localPath, bool multithread)
{
    logger->logMethodIn(__PFUNC_ID__);

    logger->debug(remotePath, "remote_path");
    QFile f(localPath);
    if (!f.open(QIODevice::ReadOnly)) {
        logger->debug(PString::format("Local file \" {LocalPath}\" open error.", {{"LocalPath", localPath}}));
        logger->logMethodOut(__PFUNC_ID__);
        return ResultType::Failure;
    }
    qint64 fileSize = f.size();
    f.close();
    // Try rapid upload first
    if (uploadFileRapid(remotePath, localPath) == ResultType::Success) {
        logger->debug(PString::format("Succeeded with uploadFileRapid, {LocalPath}", {{"LocalPath", localPath}}));
        logger->logMethodOut(__PFUNC_ID__);
        return ResultType::Success;
    }

    ResultType result;
    if (fileSize <= BaseBlockSize) {
        result = uploadFileDirect(remotePath, localPath);
        logger->debug(result, PString::format("Result for {LocalPath} via uploadFileDirect", {{"LocalPath", localPath}}));
    } else if (multithread) {
        result = uploadFileByBlockMultithread(remotePath, localPath);
        logger->debug(result, PString::format("Result for {LocalPath} via uploadFileByBlockMultithread", {{"LocalPath", localPath}}));
    } else {
        result = uploadFileByBlockSinglethread(remotePath, localPath);
        logger->debug(result, PString::format("Result for {LocalPath} via uploadFileByBlockSinglethread", {{"LocalPath", localPath}}));
    }

    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

ResultType BaiduCloudWorker::removePath(QString remotePath)
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap parameters {
        {"remote_path", QUrl::toPercentEncoding(remotePath)},
        {"access_token", settingsMap["accounts/PimixT/access_token"]}
    };

    parameters.insert(settingsMap.begin(), settingsMap.end());

    manager->executeNetworkRequest(HttpVerb::Post, PString::format(settingsMap["apis/remove_path/url"], parameters));
    ResultType result = ResultType::Success;

    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

QStringList BaiduCloudWorker::getFileList()
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap parameters {
        {"access_token", settingsMap["accounts/PimixT/access_token"]},
        {"cursor", "null"}
    };

    parameters.insert(settingsMap.begin(), settingsMap.end());

    QNetworkReply *reply = manager->executeNetworkRequest(HttpVerb::Get, PString::format(settingsMap["apis/diff_file_list/url"], parameters));

    QJsonObject diffFileListResult = QJsonDocument::fromJson(reply->readAll()).object();
    qDebug() << diffFileListResult;
    reply->deleteLater();
    QStringList result = diffFileListResult["entries"].toObject().keys();
    while (diffFileListResult["has_more"].toBool()) {
        parameters["cursor"] = diffFileListResult["cursor"].toString();
        reply = manager->executeNetworkRequest(HttpVerb::Get, PString::format(settingsMap["apis/diff_file_list/url"], parameters));
        diffFileListResult = QJsonDocument::fromJson(reply->readAll()).object();
        reply->deleteLater();
        result.append(diffFileListResult["entries"].toObject().keys());
    }
    result.replaceInStrings(QRegularExpression("^" + settingsMap["remote_path_prefix"] + "/"), "");
    logger->debug(result, "File List");
    logger->logMethodOut(__PFUNC_ID__);
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
    if (fileSize > MaxBlockCount * MaxBlockSize)
        return 0;
    qint64 blockSize = BaseBlockSize;
    while (MaxBlockCount * blockSize < fileSize)
        blockSize <<= BlockSizeIncrementalStep;

    return blockSize;
}

PStringMap BaiduCloudWorker::getFileInfos(const QString &localPath)
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap result;
    QFile f(localPath);
    if (f.open(QIODevice::ReadOnly)) {
        qint64 fileSize = f.size();
        result["content_length"] = QString::number(fileSize);
        result["slice_md5"] = QCryptographicHash::hash(f.read(256 * PFile::KilobyteSize), QCryptographicHash::Md5).toHex();
        f.seek(0);
        QCryptographicHash md5(QCryptographicHash::Md5);
        PChecksum crc(PChecksum::Algorithm::Crc32);
        QByteArray block = f.read(BaseBlockSize);
        while (!block.isEmpty()) {
            md5.addData(block);
            crc.addData(block);
            block = f.read(BaseBlockSize);
        }
        result["content_md5"] = md5.result().toHex();
        result["content_crc32"] = crc.result().toHex();
    }
    logger->debug(result, "File infos");
    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

ResultType BaiduCloudWorker::uploadFileRapid(const QString &remotePath, const QString &localPath)
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap parameters = {
        {"remote_path", QUrl::toPercentEncoding(remotePath)},
        {"access_token", settingsMap["accounts/PimixT/access_token"]}
    };

    parameters.insert(settingsMap.begin(), settingsMap.end());

    PStringMap fileInfos = getFileInfos(localPath);
    parameters.insert(fileInfos.begin(), fileInfos.end());
    auto reply = manager->executeNetworkRequest(HttpVerb::Post, PString::format(settingsMap["apis/upload_file_rapid/url"], parameters), QByteArray(), PNetworkRetryPolicy::NoRetryPolicy(600000));
    logger->debug(QString::fromUtf8(reply->readAll()));

    logger->logMethodOut(__PFUNC_ID__);
    return (reply->error() == QNetworkReply::NoError) ? ResultType::Success : ResultType::Failure;
}

ResultType BaiduCloudWorker::uploadFileDirect(QString remotePath, QString localPath)
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap parameters = {
        {"remote_path", QUrl::toPercentEncoding(remotePath)},
        {"access_token", settingsMap["accounts/PimixT/access_token"]}
    };

    parameters.insert(settingsMap.begin(), settingsMap.end());

    QFile fileToUpload(localPath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return ResultType::Failure;
    auto reply = manager->executeNetworkRequest(HttpVerb::Put, PString::format(settingsMap["apis/upload_file_direct/url"], parameters), fileToUpload.readAll());
    logger->log(QString::fromUtf8(reply->readAll()));

    ResultType result = ResultType::Success;
    logger->debug(result, "result");
    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

ResultType BaiduCloudWorker::uploadFileByBlockMultithread(QString remotePath, QString localPath)
{
    logger->logMethodIn(__PFUNC_ID__);
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
    ResultType result = mergeBlocks(remotePath, blockResultList);
    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

ResultType BaiduCloudWorker::uploadFileByBlockSinglethread(QString remotePath, QString localPath)
{
    logger->logMethodIn(__PFUNC_ID__);
    QFile fileToUpload(localPath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return ResultType::Failure;
    QStringList blockHashList;

    QByteArray data = fileToUpload.read(BaseBlockSize);
    while (!data.isEmpty()) {
        bc = 0;
        blockHashList.append(uploadBlock(data));
        logger->debug(blockHashList, "Finished blocks");
        logger->debug(blockHashList.count(), "Finished blocks count");
        data = fileToUpload.read(BaseBlockSize);
    }
    ResultType result = mergeBlocks(remotePath, blockHashList);
    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

bool BaiduCloudWorker::verifyFile(const QString &remotePath)
{
    PStringMap parameters = {
        {"remote_path", QUrl::toPercentEncoding(remotePath)},
        {"access_token", settingsMap["accounts/PimixT/access_token"]}
    };

    parameters.insert(settingsMap.begin(), settingsMap.end());

    manager->executeNetworkRequest(HttpVerb::Post, PString::format(settingsMap["apis/verify_file/url"], parameters));
    return false;
}

QString BaiduCloudWorker::uploadBlock(const QByteArray &data)
{
    logger->logMethodIn(__PFUNC_ID__);
    logger->debug(data.size(), "UploadBlockSize");
    PStringMap parameters = {
        {"access_token", settingsMap["accounts/PimixT/access_token"]}
    };

    parameters.insert(settingsMap.begin(), settingsMap.end());

    PNetworkAccessManager m;
    m.setRetryPolicy(PNetworkRetryPolicy::UnlimitedRetryPolicy(1200000));
    QNetworkReply *reply = m.executeNetworkRequest(HttpVerb::Put, PString::format(settingsMap["apis/upload_block/url"], parameters), data);
    auto result = reply->readAll();
    reply->deleteLater();
    QJsonObject blockUploadResult = QJsonDocument::fromJson(result).object();
    logger->debug(QString::fromUtf8(result), "Block upload result");
    logger->logMethodOut(__PFUNC_ID__);
    return blockUploadResult["md5"].toString();
}

ResultType BaiduCloudWorker::mergeBlocks(QString remotePath, QStringList blockHashList)
{
    logger->logMethodIn(__PFUNC_ID__);

    PStringMap parameters = {
        {"remote_path", QUrl::toPercentEncoding(remotePath)},
        {"access_token", settingsMap["accounts/PimixT/access_token"]}
    };

    parameters.insert(settingsMap.begin(), settingsMap.end());

    QJsonObject param;
    param["block_list"] = QJsonArray::fromStringList(blockHashList);
    QNetworkRequest request(PString::format(settingsMap["apis/merge_blocks/url"], parameters));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    auto reply = manager->executeNetworkRequest(HttpVerb::Post, request, QByteArray("param=") + QJsonDocument(param).toJson());

    logger->debug(QString::fromUtf8(reply->readAll()));

    logger->logMethodOut(__PFUNC_ID__);
    return (reply->error() == QNetworkReply::NoError) ? ResultType::Success : ResultType::Failure;
}
