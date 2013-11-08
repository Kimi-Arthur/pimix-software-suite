#ifndef PTNETWORK_H
#define PTNETWORK_H

#include "PtNetwork_global.h"
//#include <QNetworkProxy>
#include <QString>
#include <QRegularExpression>


namespace Pt {
namespace Network {

//PTNETWORKSHARED_EXPORT QNetworkProxy getProxy(const QString &proxyString);
enum class HttpVerb
{
    Get,
    Head,
    Post,
    Put,
    Delete,
    Trace,
    Options,
    Connect,
    Patch
};

}
}

#endif // PTNETWORK_H
