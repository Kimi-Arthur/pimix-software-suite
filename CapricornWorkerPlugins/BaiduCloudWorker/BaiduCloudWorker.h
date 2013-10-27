#ifndef BAIDUCLOUDWORKER_H
#define BAIDUCLOUDWORKER_H

#include "PLogger.h"
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

using namespace Pt::Core;

using namespace Pt::Network;

class BAIDUCLOUDWORKERSHARED_EXPORT BaiduCloudWorker : public QObject, public BaseCapricornWorker
{
    // Variables
    QString accessToken;
    QMap<QString, QString> requestUrlPatterns;
    static const qint64 BaseBlockSize = 32 * PFile::MegabyteSize;
    static const qint64 MaxBlockSize = 2 * PFile::GigabyteSize;
    static const qint64 MaxBlockCount = 1024;
    static const qint64 MaxThreadCount = 8;
    QJsonObject settings;
    PNetworkAccessManager *manager = new PNetworkAccessManager();
    // Temp variables
    qint64 bc;
    PLogger *logger;
public:
    BaiduCloudWorker(PLogger *_logger = new PLogger());
    ResultType downloadFile(QString remotePath, QString localPath);
    ResultType uploadFile(QString remotePath, QString localPath);
    ResultType removePath(QString remotePath);
private slots:
    void showProgress(qint64 bs, qint64 bt);
private:
    qint64 static getBlockSize(qint64 fileSize);
    ResultType uploadFileDirect(QString remotePath, QString localPath);
    ResultType uploadFileByBlockMultithread(QString remotePath, QString localPath);
    ResultType uploadFileByBlockSinglethread(QString remotePath, QString localPath);
    QString uploadBlock(const QByteArray &data);
    QString uploadBlockFromFile(const QString &filePath, qint64 startIndex, qint64 length);
    QString uploadBlockFromMyFile(const std::tuple<QString, qint64, qint64> &parameters);
    ResultType mergeBlocks(QString remotePath, QStringList blockHashList);
};

class BAIDUCLOUDWORKERSHARED_EXPORT BaiduCloudWorkerFactory : public QObject, public BaseCapricornWorkerFactory
{
public:
    BaiduCloudWorkerFactory();
};

#endif // BAIDUCLOUDWORKER_H
