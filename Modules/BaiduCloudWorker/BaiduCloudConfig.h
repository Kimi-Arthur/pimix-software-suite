#ifndef BAIDUCLOUDCONFIG_H
#define BAIDUCLOUDCONFIG_H

#include "PConfig.h"

#include "baiducloudworker_global.h"

#include <QList>
#include "PtNetwork.h"

class BaiduCloudApi
{
    QString url;
    Pt::Network::HttpVerb verb;
};

class BaiduCloudConfig : PConfig
{
public:
    BaiduCloudConfig();

    // Data Model
    QMap<QString, BaiduCloudApi> apis;
};

#endif // BAIDUCLOUDCONFIG_H
