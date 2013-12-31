#include "PJsonValue.h"
#include <QCoreApplication>
#include <QDir>
#include <QStringList>
#include <QThread>

#include "BaiduCloudAccount.h"

BaiduCloudAccount *w = BaiduCloudAccount::getInstanceByName("PimixT");

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
    QStringList onlineList = w->getFileList();
    QStringList offlineList = getLocalFileList(baseLocalPath);

    onlineList.replaceInStrings(QRegularExpression("^Arch/"), "");
    offlineList.replaceInStrings(QRegularExpression("^" + baseLocalPath + "/"), "");

    qDebug() << offlineList;

    foreach (auto online, onlineList) {
        offlineList.removeAll(online);
    }

    return offlineList;
}

void uploadFile(const QString &baseLocalPath, const QString &relativePath)
{
    w->uploadFile("Arch/" + relativePath, baseLocalPath + "/" + relativePath);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString basePath = argv[1];
    qDebug() << basePath;
    while (true) {
        auto fl = getUpdatedFileList(basePath);
        foreach (auto f, fl) {
            uploadFile(basePath, f);
        }
        QThread::sleep(1800);
    }
}
