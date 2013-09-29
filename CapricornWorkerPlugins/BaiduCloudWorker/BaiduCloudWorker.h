#ifndef BAIDUCLOUDWORKER_H
#define BAIDUCLOUDWORKER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QNetworkAccessManager>
#include "baiducloudworker_global.h"
#include "PFile.h"
#include "PtCore.h"
#include "BaseCapricornWorker.h"
#include "PtNetwork.h"
#include "map"
#include "PNetworkAccessManager.h"

using namespace Pt::Network;

class BAIDUCLOUDWORKERSHARED_EXPORT BaiduCloudWorker : public QObject, public BaseCapricornWorker
{
    // Variables
    QString accessToken;
    QMap<QString, QString> requestUrlPatterns;
    static const qint64 baseBlockSize = 8 * PFile::MegabyteSize;
    static const qint64 maxBlockSize = 2 * PFile::GigabyteSize;
    static const qint64 maxBlockCount = 1024;
    QJsonObject settings;
    PNetworkAccessManager *manager = new PNetworkAccessManager();
    // Temp variables
    qint64 bc;
public:
    BaiduCloudWorker();
    ResultType downloadFile(QString remotePath, QString localPath);
    ResultType uploadFile(QString remotePath, QString localPath);
    ResultType removePath(QString remotePath);
private slots:
    void showProgress(qint64 bs, qint64 bt);
private:
    qint64 static getBlockSize(qint64 fileSize);
    ResultType uploadFileDirect(QString remotePath, QString localPath);
    ResultType uploadFileByBlock(QString remotePath, QString localPath);
    QString uploadBlock(const QByteArray &data);
    ResultType mergeBlocks(QString remotePath, QStringList blockHashList);
};

class BAIDUCLOUDWORKERSHARED_EXPORT BaiduCloudWorkerFactory : public QObject, public BaseCapricornWorkerFactory
{
public:
    BaiduCloudWorkerFactory();
};

#endif // BAIDUCLOUDWORKER_H
