#include "ptnetwork.h"


QNetworkProxy Pt::getProxy(const QString &proxyString)
{
    auto result = QRegularExpression("^((?<username>.*):(?<password>.*)@)?(?<hostname>[\\.a-zA-Z0-9\\-]+):(?<port>\\d+)$").match(proxyString);
    if (result.hasMatch())
        return QNetworkProxy(QNetworkProxy::HttpProxy, result.captured("hostname"), result.captured("port").toInt(), result.captured("username"), result.captured("password"));
    else return QNetworkProxy();
}
