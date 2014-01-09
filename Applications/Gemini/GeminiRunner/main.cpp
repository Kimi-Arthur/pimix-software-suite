#include "PJsonValue.h"
#include <QCoreApplication>
#include <QDir>
#include <QStringList>
#include <QThread>

#include "BaiduCloudService.h"
#include "BaiduCloudAccount.h"

PLogger logger;

BaiduCloudService s("baidu_cloud.pc", &logger);

QStringList getLocalFileList(const QString &baseLocalPath)
{
    logger.logMethodIn(__PFUNC_ID__);

    QDir d(baseLocalPath);
    QStringList result = d.entryList(QDir::Files).replaceInStrings(QRegularExpression("^"), baseLocalPath + "/");
    foreach (auto de, d.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        result.append(getLocalFileList(baseLocalPath + "/" + de));
    }

    logger.logMethodOut(__PFUNC_ID__);
    return result;
}

QStringList getUpdatedFileList(const QString &baseLocalPath)
{
    logger.logMethodIn(__PFUNC_ID__);

    QStringList offlineList = getLocalFileList(baseLocalPath);

    offlineList.replaceInStrings(QRegularExpression("^" + baseLocalPath + "/"), "");

    qDebug() << offlineList;

    logger.logMethodOut(__PFUNC_ID__);
    return offlineList;
}

void uploadFile(const QString &baseLocalPath, const QString &relativePath)
{
    logger.logMethodIn(__PFUNC_ID__);

    s.getAccountByPath(relativePath)->uploadFile(relativePath, baseLocalPath + "/" + relativePath);

    logger.logMethodOut(__PFUNC_ID__);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString basePath = argv[1];

    logger.info(basePath, "Base local path");

    while (true) {
        logger.info("One round of sync started!");
        auto fl = getUpdatedFileList(basePath);
        foreach (auto f, fl) {
            try {
                uploadFile(basePath, f);
            } catch (...) {
                logger.error("Upload for " + basePath + " failed!");
            }
        }
        logger.info("One round of sync done!");
        logger.info("Sleep 30 mins.");
        QThread::sleep(1800);
    }
}
