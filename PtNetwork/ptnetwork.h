#ifndef PTNETWORK_H
#define PTNETWORK_H

#include "PtNetwork_global.h"
#include <QNetworkProxy>
#include <QString>
#include <QRegularExpression>

namespace Pt {
PTNETWORKSHARED_EXPORT QNetworkProxy getProxy(const QString &proxyString);
}

#endif // PTNETWORK_H
