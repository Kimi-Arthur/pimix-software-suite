#ifndef BAIDUCLOUDSERVICE_H
#define BAIDUCLOUDSERVICE_H

#include "BaiduCloud_global.h"

#include "BaiduCloudAccount.h"

class BAIDUCLOUDSHARED_EXPORT BaiduCloudService
{
public:
    explicit BaiduCloudService(const QString &settingsFileName, PLogger *logger);

    BaiduCloudAccount *getAccountByPath(const QString &remotePath);
    BaiduCloudAccount *getAccountByName(const QString &accountName);

private:
    PLogger *logger;
    PJsonValue settings;
    PStringMap settingsMap;
    QMap<QString, BaiduCloudAccount *> existingAccounts;

};

#endif // BAIDUCLOUDSERVICE_H
