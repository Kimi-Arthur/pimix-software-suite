#ifndef BAIDUCLOUDWORKER_H
#define BAIDUCLOUDWORKER_H

#include "baiducloudworker_global.h"
#include "BaseCapricornWorker.h"

class BAIDUCLOUDWORKERSHARED_EXPORT BaiduCloudWorker : public QObject, public BaseCapricornWorker
{
    // Variables
    QString accessToken;
    QMap<QString, QString> requestUrlPatterns;
public:
    BaiduCloudWorker();
    ResultType downloadFile(QString remotePath, QString localPath);
    ResultType uploadFile(QString remotePath, QString localPath);
private:

    ResultType uploadHugeFile(QString remotePath, QString localPath);
};

class BAIDUCLOUDWORKERSHARED_EXPORT BaiduCloudWorkerFactory : public QObject, public BaseCapricornWorkerFactory
{
public:
    BaiduCloudWorkerFactory();
};

#endif // BAIDUCLOUDWORKER_H
