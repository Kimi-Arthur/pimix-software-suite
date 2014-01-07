#ifndef BAIDUCLOUDACCOUNT_H
#define BAIDUCLOUDACCOUNT_H

#include "BaiduCloud_global.h"

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

    explicit BaiduCloudAccount(const BaiduCloudAccountInfo &accountInfo, PJsonValue *settings, PStringMap *settingsMap, PLogger *logger);
    ResultType downloadFile(QString remotePath, QString localPath);
    ResultType uploadFile(QString remotePath, QString localPath, bool multithread = false);
    ResultType removePath(QString remotePath);
    QStringList getFileList();

private:
    PLogger *logger;
    PJsonValue *settings;
    PStringMap *settingsMap;

private:
    // Variables
    BaiduCloudAccountInfo accountInfo;
    QMap<QString, QString> requestUrlPatterns;
    const qint64 BaseBlockSize = 32 * PFile::MegabyteSize;
    const int BlockSizeIncrementalStep = 2;
    const qint64 MinRapidUploadSize = 256 * PFile::KilobyteSize;
    const qint64 MaxBlockSize = 2 * PFile::GigabyteSize;
    const qint64 MaxBlockCount = 1024;
    const qint64 MaxThreadCount = 8;
    PNetworkAccessManager *manager = new PNetworkAccessManager();

    QStringList currentFileList;
    QString currentFileListCursor = "null";
    // Temp variables
    qint64 bc;

private slots:
    void showProgress(qint64 bs, qint64 bt);

private:
    qint64 getBlockSize(qint64 fileSize);
    bool diffFileList();
    PStringMap getFileInfos(const QString &localPath);
    ResultType uploadFileRapid(const QString &remotePath, const QString &localPath);
    ResultType uploadFileDirect(QString remotePath, QString localPath);
    ResultType uploadFileByBlockMultithread(QString remotePath, QString localPath);
    ResultType uploadFileByBlockSinglethread(QString remotePath, QString localPath);
    bool pathExists(const QString &remotePath);
    bool verifyFile(const QString &remotePath);
    QString uploadBlock(const QByteArray &data);
    ResultType mergeBlocks(QString remotePath, QStringList blockHashList);
};


#endif // BAIDUCLOUDACCOUNT_H
