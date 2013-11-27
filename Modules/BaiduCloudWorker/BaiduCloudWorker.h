#ifndef BAIDUCLOUDWORKER_H
#define BAIDUCLOUDWORKER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include "baiducloudworker_global.h"
#include "PFile.h"
#include "PtCore.h"
#include "PtNetwork.h"
#include "PLogger.h"
#include "PString.h"
#include <map>
#include "PNetworkAccessManager.h"

using namespace Pt::Core;

using namespace Pt::Network;

class BAIDUCLOUDWORKERSHARED_EXPORT BaiduCloudWorker : public QObject
{
    // Variables
    QString accessToken;
    QMap<QString, QString> requestUrlPatterns;
    const qint64 BaseBlockSize = 32 * PFile::MegabyteSize;
    const int BlockSizeIncrementalStep = 2;
    const qint64 MaxBlockSize = 2 * PFile::GigabyteSize;
    const qint64 MaxBlockCount = 1024;
    const qint64 MaxThreadCount = 8;
    QJsonObject settings;
    PNetworkAccessManager *manager = new PNetworkAccessManager();
    // Temp variables
    qint64 bc;
    PLogger *logger;

public:
    BaiduCloudWorker(PLogger *_logger = new PLogger());
    ResultType downloadFile(QString remotePath, QString localPath);
    ResultType uploadFile(QString remotePath, QString localPath, bool multithread = false);
    ResultType removePath(QString remotePath);
    QStringList getFileList();

private slots:
    void showProgress(qint64 bs, qint64 bt);

private:
    qint64 getBlockSize(qint64 fileSize);
    std::map<QString, QString> getFileInfos(const QString &localPath);
    ResultType uploadFileRapid(const QString &remotePath, const QString &localPath);
    ResultType uploadFileDirect(QString remotePath, QString localPath);
    ResultType uploadFileByBlockMultithread(QString remotePath, QString localPath);
    ResultType uploadFileByBlockSinglethread(QString remotePath, QString localPath);
    bool verifyFile(const QString &remotePath);
    QString uploadBlock(const QByteArray &data);
    ResultType mergeBlocks(QString remotePath, QStringList blockHashList);
};


#endif // BAIDUCLOUDWORKER_H
