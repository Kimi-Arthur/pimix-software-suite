#include "BaiduCloudAccount.h"
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

BaiduCloudAccount::BaiduCloudAccount(const BaiduCloudAccountInfo &accountInfo, PJsonValue *settings, PStringMap *settingsMap, PLogger *logger)
    : accountInfo(accountInfo),
      settings(settings),
      settingsMap(settingsMap),
      logger(logger)
{
    this->logger->logMethodIn(__PFUNC_ID__);

    manager = new PNetworkAccessManager(this->logger);
    manager->setRetryPolicy(PNetworkRetryPolicy::LimitedRetryPolicy(5, 600000));

    this->logger->debug(accountInfo.username, "username");

    this->logger->logMethodOut(__PFUNC_ID__);
}

ResultType BaiduCloudAccount::downloadFile(QString remotePath, QString localPath)
{
    return downloadFileDirect(remotePath, localPath);
}

ResultType BaiduCloudAccount::uploadFile(QString remotePath, QString localPath, bool multithread)
{
    logger->logMethodIn(__PFUNC_ID__);

    logger->debug(remotePath, "remote_path");

    if (pathExists(remotePath))
    {
        logger->debug(PString::format("Remote path \"{remote_path}\" already exists!", {{"remote_path", remotePath}}));
        logger->logMethodOut(__PFUNC_ID__);
        return ResultType::Failure;
    }

    QFile f(localPath);
    if (!f.open(QIODevice::ReadOnly)) {
        logger->debug(PString::format("Local file \"{local_path}\" open error!", {{"local_path", localPath}}));
        logger->logMethodOut(__PFUNC_ID__);
        return ResultType::Failure;
    }
    qint64 fileSize = f.size();
    f.close();
    // Try rapid upload first
    if (uploadFileRapid(remotePath, localPath) == ResultType::Success) {
        logger->debug(PString::format("Succeeded with uploadFileRapid, {local_path}", {{"local_path", localPath}}));
        logger->logMethodOut(__PFUNC_ID__);
        return ResultType::Success;
    }

    ResultType result;
    if (fileSize <= BaseBlockSize) {
        result = uploadFileDirect(remotePath, localPath);
        logger->debug(result, PString::format("Result for {local_path} via uploadFileDirect", {{"local_path", localPath}}));
    } else if (multithread) {
        result = uploadFileByBlockMultithread(remotePath, localPath);
        logger->debug(result, PString::format("Result for {local_path} via uploadFileByBlockMultithread", {{"local_path", localPath}}));
    } else {
        result = uploadFileByBlockSinglethread(remotePath, localPath);
        logger->debug(result, PString::format("Result for {local_path} via uploadFileByBlockSinglethread", {{"local_path", localPath}}));
    }

    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

ResultType BaiduCloudAccount::removePath(QString remotePath)
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap parameters {
        {"remote_path", QUrl::toPercentEncoding(remotePath)},
        {"access_token", accountInfo.accessToken}
    };

    parameters.insert(settingsMap->begin(), settingsMap->end());

    manager->executeNetworkRequest(HttpVerb::Post, PString::format(settingsMap->at("apis/remove_path/url"), parameters));
    ResultType result = ResultType::Success;

    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

QStringList BaiduCloudAccount::getFileList()
{
    logger->logMethodIn(__PFUNC_ID__);

    while (diffFileList());

    logger->debug(currentFileList.size(), "Current file list size");

    logger->logMethodOut(__PFUNC_ID__);
    return currentFileList;
}

void BaiduCloudAccount::showProgress(qint64 bs, qint64 bt)
{
    if (bs - bc > 500 * PFile::KilobyteSize) {
        qDebug() << "Progress" << bs << bt;
        bc = bs;
    }
}

bool BaiduCloudAccount::diffFileList()
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap parameters {
        {"access_token", accountInfo.accessToken},
        {"cursor", currentFileListCursor}
    };

    parameters.insert(settingsMap->begin(), settingsMap->end());

    QNetworkReply *reply = manager->executeNetworkRequest(HttpVerb::Get, PString::format(settingsMap->at("apis/diff_file_list/url"), parameters));

    QJsonObject diffFileListResult = QJsonDocument::fromJson(reply->readAll()).object();
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
        throw BaiduCloudUploadException();

    currentFileListCursor = diffFileListResult["cursor"].toString();

    if (diffFileListResult["reset"].toBool())
        currentFileList.clear();

    foreach (auto v, diffFileListResult["entries"].toObject()) {
        QJsonObject properties = v.toObject();
        if (properties["isdir"].toInt() == 1)
            continue;

        QString path = v.toObject()["path"].toString().remove(QRegularExpression("^" + settingsMap->at("remote_path_prefix") + "/"));
        if (properties["isdelete"] == 0)
            currentFileList.append(path);
        else {
            int removedCount = currentFileList.removeAll(path);
            logger->logAssertEquals(1, removedCount, "File item removed");
        }
    }

    logger->logMethodOut(__PFUNC_ID__);
    return diffFileListResult["has_more"].toBool();
}

PStringMap BaiduCloudAccount::getFileInfos(const QString &localPath)
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

ResultType BaiduCloudAccount::uploadFileRapid(const QString &remotePath, const QString &localPath)
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap parameters {
        {"remote_path", QUrl::toPercentEncoding(remotePath)},
        {"access_token", accountInfo.accessToken}
    };

    parameters.insert(settingsMap->begin(), settingsMap->end());

    PStringMap fileInfos = getFileInfos(localPath);
    parameters.insert(fileInfos.begin(), fileInfos.end());

    if (fileInfos["content_length"].toLongLong() < MinRapidUploadSize) {
        logger->debug("Rapid upload fail because file size is too small.");
        logger->logMethodOut(__PFUNC_ID__);
        return ResultType::Failure;
    }

    auto reply = manager->executeNetworkRequest(HttpVerb::Post, PString::format(settingsMap->at("apis/upload_file_rapid/url"), parameters), QByteArray(), PNetworkRetryPolicy::NoRetryPolicy(600000));
    logger->debug(QString::fromUtf8(reply->readAll()));

    auto result = (reply->error() == QNetworkReply::NoError) ? ResultType::Success : ResultType::Failure;

    reply->deleteLater();

    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

ResultType BaiduCloudAccount::uploadFileDirect(QString remotePath, QString localPath)
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap parameters {
        {"remote_path", QUrl::toPercentEncoding(remotePath)},
        {"access_token", accountInfo.accessToken}
    };

    parameters.insert(settingsMap->begin(), settingsMap->end());

    QFile fileToUpload(localPath);
    if (!fileToUpload.open(QIODevice::ReadOnly))
        return ResultType::Failure;
    auto reply = manager->executeNetworkRequest(HttpVerb::Put, PString::format(settingsMap->at("apis/upload_file_direct/url"), parameters), fileToUpload.readAll());
    logger->debug(QString::fromUtf8(reply->readAll()));

    ResultType result = ResultType::Success;
    logger->debug(result, "result");
    reply->deleteLater();

    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

ResultType BaiduCloudAccount::uploadFileByBlockMultithread(QString remotePath, QString localPath)
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
        blockStatusList.append(QtConcurrent::run(this, &BaiduCloudAccount::uploadBlock, data));
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

ResultType BaiduCloudAccount::uploadFileByBlockSinglethread(QString remotePath, QString localPath)
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

ResultType BaiduCloudAccount::downloadFileDirect(QString remotePath, QString localPath)
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap parameters {
        {"access_token", accountInfo.accessToken},
        {"remote_path", QUrl::toPercentEncoding(remotePath)}
    };

    parameters.insert(settingsMap->begin(), settingsMap->end());

    PNetworkAccessManager m(logger);
    m.setRetryPolicy(PNetworkRetryPolicy::UnlimitedRetryPolicy(1800000, 600000));
    QNetworkReply *reply = m.executeNetworkRequest(HttpVerb::Get, PString::format(settingsMap->at("apis/download_file/url"), parameters));
    auto result = reply->readAll();
    reply->deleteLater();

    logger->debug(result.size(), "Downloaded size");
    logger->logMethodOut(__PFUNC_ID__);
    return ResultType::Failure;
}

bool BaiduCloudAccount::pathExists(const QString &remotePath)
{
    logger->logMethodIn(__PFUNC_ID__);

    QStringList fileList = getFileList();

    logger->logMethodOut(__PFUNC_ID__);
    return fileList.contains(remotePath);
}

bool BaiduCloudAccount::verifyFile(const QString &remotePath)
{
    PStringMap parameters {
        {"remote_path", QUrl::toPercentEncoding(remotePath)},
        {"access_token", accountInfo.accessToken}
    };

    parameters.insert(settingsMap->begin(), settingsMap->end());

    manager->executeNetworkRequest(HttpVerb::Post, PString::format(settingsMap->at("apis/verify_file/url"), parameters));
    return false;
}

QString BaiduCloudAccount::uploadBlock(const QByteArray &data)
{
    logger->logMethodIn(__PFUNC_ID__);
    logger->debug(data.size(), "UploadBlockSize");
    PStringMap parameters {
        {"access_token", accountInfo.accessToken}
    };

    parameters.insert(settingsMap->begin(), settingsMap->end());

    PNetworkAccessManager m(logger);
    m.setRetryPolicy(PNetworkRetryPolicy::UnlimitedRetryPolicy(1800000, 600000));
    QNetworkReply *reply = m.executeNetworkRequest(HttpVerb::Put, PString::format(settingsMap->at("apis/upload_block/url"), parameters), data);
    auto result = reply->readAll();
    reply->deleteLater();
    QJsonObject blockUploadResult = QJsonDocument::fromJson(result).object();
    logger->debug(QString::fromUtf8(result), "Block upload result");
    logger->logMethodOut(__PFUNC_ID__);
    return blockUploadResult["md5"].toString();
}

QByteArray BaiduCloudAccount::downloadBlock(const QString &remotePath, qint64 startIndex, qint64 blockSize)
{
    logger->logMethodIn(__PFUNC_ID__);
    PStringMap parameters {
        {"access_token", accountInfo.accessToken},
        {"remote_path", QUrl::toPercentEncoding(remotePath)}
    };

    parameters.insert(settingsMap->begin(), settingsMap->end());

    PNetworkAccessManager m(logger);
    m.setRetryPolicy(PNetworkRetryPolicy::UnlimitedRetryPolicy(1800000, 600000));
    QNetworkRequest request(PString::format(settingsMap->at("apis/download_file/url"), parameters));
    request.setRawHeader(
                "Range",
                PString::format(
                    "bytes={0}-{1}",
                    {
                        { "0", QString::number(startIndex) },
                        { "1", QString::number(startIndex + blockSize - 1) }
                    }).toUtf8());
    QNetworkReply *reply = m.executeNetworkRequest(HttpVerb::Get, request);
    auto result = reply->readAll();
    reply->deleteLater();

    logger->debug(result.size(), "Downloaded size");
    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

ResultType BaiduCloudAccount::mergeBlocks(QString remotePath, QStringList blockHashList)
{
    logger->logMethodIn(__PFUNC_ID__);

    PStringMap parameters {
        {"remote_path", QUrl::toPercentEncoding(QUrl::toPercentEncoding(remotePath))},
        {"access_token", accountInfo.accessToken}
    };

    parameters.insert(settingsMap->begin(), settingsMap->end());

    QJsonObject param;
    param["block_list"] = QJsonArray::fromStringList(blockHashList);
    QNetworkRequest request(PString::format(settingsMap->at("apis/merge_blocks/url"), parameters));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    auto reply = manager->executeNetworkRequest(HttpVerb::Post, request, QByteArray("param=") + QJsonDocument(param).toJson());

    logger->debug(QString::fromUtf8(reply->readAll()));

    auto result = (reply->error() == QNetworkReply::NoError) ? ResultType::Success : ResultType::Failure;

    reply->deleteLater();
    logger->logMethodOut(__PFUNC_ID__);
    return result;
}
