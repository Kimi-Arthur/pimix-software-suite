#include "PJsonValue.h"
#include <QCoreApplication>
#include <QDir>
#include <QStringList>
#include <QThread>

#include "BaiduCloudService.h"
#include "BaiduCloudAccount.h"

BaiduCloudService s("baidu_cloud.pc");
PLogger logger;

QStringList getLocalFileList(const QString &baseLocalPath)
{
    QDir d(baseLocalPath);
    QStringList result = d.entryList(QDir::Files).replaceInStrings(QRegularExpression("^"), baseLocalPath + "/");
    foreach (auto de, d.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        result.append(getLocalFileList(baseLocalPath + "/" + de));
    }
    return result;
}

QStringList getUpdatedFileList(const QString &baseLocalPath)
{
    QStringList offlineList = getLocalFileList(baseLocalPath);

    offlineList.replaceInStrings(QRegularExpression("^" + baseLocalPath + "/"), "");

    qDebug() << offlineList;

    return offlineList;
}

void uploadFile(const QString &baseLocalPath, const QString &relativePath)
{
    s.getAccountByPath(relativePath)->uploadFile(relativePath, baseLocalPath + "/" + relativePath);
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
            uploadFile(basePath, f);
        }
        logger.info("One round of sync done! Sleep 30 mins.");
        QThread::sleep(1800);
    }
}
