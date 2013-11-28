#ifndef BAIDUCLOUDCONFIG_H
#define BAIDUCLOUDCONFIG_H

#include "PConfig.h"

#include "baiducloudworker_global.h"

#include <QList>
#include "PtNetwork.h"

class BaiduCloudApi : Pt::Core::PDataModel
{
public:
    BaiduCloudApi();

    DataMember()
    QString url;

    DataMember()
    Pt::Network::HttpVerb method;
};

class BaiduCloudConfig : Pt::Core::PDataModel
{
public:
    BaiduCloudConfig();

    DataMember()
    QString name;

    DataMember()
    QString title;

    DataMember(client_secret)
    QString clientSecret;

    DataMember(client_id)
    QString clientId;

    DataMember(remote_path_prefix)
    QString remotePathPrefix;

    DataMember()
    QMap<QString, BaiduCloudApi> apis;
};

#endif // BAIDUCLOUDCONFIG_H
