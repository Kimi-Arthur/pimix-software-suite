#ifndef BAIDUCLOUDACCOUNT_H
#define BAIDUCLOUDACCOUNT_H

#include "BaiduCloud_global.h"
#include "PFile.h"
#include "PtCore.h"
#include "PtNetwork.h"
#include "PJsonValue.h"
#include "PLogger.h"
#include "PString.h"
#include <map>
#include "PNetworkAccessManager.h"

using namespace Pt::Core;

using namespace Pt::Network;

class BAIDUCLOUDSHARED_EXPORT BaiduCloudAccountInfo
{
public:
    explicit BaiduCloudAccountInfo(const PJsonValue &value)
    {
        qDebug() << value;
        auto m = value.toMap();
        username = m["username"];
        accessToken = m["access_token"];
    }

    QString username;
    QString accessToken;
};

class BAIDUCLOUDSHARED_EXPORT BaiduCloudAccount : public QObject
{

public:
    static BaiduCloudAccount *getInstanceByPath(const QString &remotePath);
    static BaiduCloudAccount *getInstanceByName(const QString &accountName);

    explicit BaiduCloudAccount(const QString &accountName);
    ResultType downloadFile(QString remotePath, QString localPath);
    ResultType uploadFile(QString remotePath, QString localPath, bool multithread = false);
    ResultType removePath(QString remotePath);
    QStringList getFileList();

private:
    static void initialize();
    static PLogger *logger;
    static PJsonValue *settings;
    static PStringMap *settingsMap;
    static QMap<QString, BaiduCloudAccount *> existingAccounts;

private:
    // Variables
    BaiduCloudAccountInfo accountInfo;
    QMap<QString, QString> requestUrlPatterns;
    const qint64 BaseBlockSize = 32 * PFile::MegabyteSize;
    const int BlockSizeIncrementalStep = 2;
    const qint64 MaxBlockSize = 2 * PFile::GigabyteSize;
    const qint64 MaxBlockCount = 1024;
    const qint64 MaxThreadCount = 8;
    PNetworkAccessManager *manager = new PNetworkAccessManager();
    // Temp variables
    qint64 bc;
    const static QString settingsFileName;

private slots:
    void showProgress(qint64 bs, qint64 bt);

private:
    qint64 getBlockSize(qint64 fileSize);
    PStringMap getFileInfos(const QString &localPath);
    ResultType uploadFileRapid(const QString &remotePath, const QString &localPath);
    ResultType uploadFileDirect(QString remotePath, QString localPath);
    ResultType uploadFileByBlockMultithread(QString remotePath, QString localPath);
    ResultType uploadFileByBlockSinglethread(QString remotePath, QString localPath);
    bool verifyFile(const QString &remotePath);
    QString uploadBlock(const QByteArray &data);
    ResultType mergeBlocks(QString remotePath, QStringList blockHashList);
};


#endif // BAIDUCLOUDACCOUNT_H
