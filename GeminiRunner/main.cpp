#include <QCoreApplication>
#include <QStringList>
#include <QThread>

#include "BaiduCloudWorker.h"

BaiduCloudWorker w;

QStringList getLocalFileList(const QString &baseLocalPath)
{
    return QStringList();
}

QStringList getUpdatedFileList(const QString &baseLocalPath)
{
    QStringList onlineList = w.getFileList();
    QStringList offlineList = getLocalFileList(baseLocalPath);
    foreach (auto online, onlineList) {
        offlineList.removeAll(online);
    }

    return offlineList;
}

void uploadFile(const QString &baseLocalPath, const QString &relativePath)
{
    w.uploadFile("Backup/" + relativePath, baseLocalPath + "/" + relativePath);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    QString basePath = argv[1];
    QString basePath = "U:/";
    while (true) {
        auto fl = getUpdatedFileList(basePath);
        foreach (auto f, fl) {
            uploadFile(basePath, f);
        }
        QThread::sleep(1800);
    }
}
