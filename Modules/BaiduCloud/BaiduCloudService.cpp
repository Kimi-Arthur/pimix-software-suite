#include "BaiduCloudService.h"

#include <QEventLoop>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtMath>
#include <QJsonArray>
#include <QPair>
#include <QTimer>
#include <QStringList>
#include <QtConcurrent/QtConcurrent>
#include "PLogger.h"
#include "PChecksum.h"

BaiduCloudService::BaiduCloudService(const QString &settingsFileName, PLogger *logger)
    : logger(logger)
{
    QFile settingsFile(settingsFileName);
    if (!settingsFile.open(QIODevice::ReadOnly)) {
        logger->debug(PString::format("Settings file {SettingsFile} not found!", {{"SettingsFile", settingsFileName}}));
        return;
    }
    auto value = settingsFile.readAll();
    settings = PJsonValue(QJsonDocument::fromJson(value).object());
    settingsMap = settings.toMap();
}


BaiduCloudAccount *BaiduCloudService::getAccountByPath(const QString &remotePath)
{
    logger->logMethodIn(__PFUNC_ID__);
    int maxCapturedLength = -1;
    QString accountName;
    foreach (auto mapping, settings["mappings"].toArray()) {
        auto value = PJsonValue(mapping).toMap();
        int len = QRegularExpression(value["path"]).match(remotePath).capturedLength();
        if (len > maxCapturedLength)
        {
            maxCapturedLength = len;
            accountName = value["account"];
        }
    }

    logger->debug(accountName, "Selected account");
    logger->debug(maxCapturedLength, "Captured length");
    auto result = getAccountByName(accountName);

    logger->logMethodOut(__PFUNC_ID__);
    return result;
}

BaiduCloudAccount *BaiduCloudService::getAccountByName(const QString &accountName)
{
    logger->logMethodIn(__PFUNC_ID__);

    auto item = existingAccounts.find(accountName);
    if (item == existingAccounts.end()) {
        logger->debug("No item for " + accountName + " exists!");
        item = existingAccounts.insert(accountName,
                                       new BaiduCloudAccount(BaiduCloudAccountInfo(settings["accounts"][accountName]),
                                       &settings, &settingsMap, logger));
    }

    logger->logMethodOut(__PFUNC_ID__);
    return item.value();
}
