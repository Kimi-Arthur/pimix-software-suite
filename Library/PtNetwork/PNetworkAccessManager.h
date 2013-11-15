#ifndef PNETWORKACCESSMANAGER_H
#define PNETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>
#include "PNetworkRetryPolicy.h"
#include "PtNetwork.h"
#include "PtNetwork_global.h"
#include <map>


namespace Pt {
namespace Network {

class PTNETWORKSHARED_EXPORT PNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
    PNetworkRetryPolicy *defaultRetryPolicy;
public:
    explicit PNetworkAccessManager(QObject *parent = 0);
    void setRetryPolicy(PNetworkRetryPolicy *retryPolicy);
    QNetworkReply *executeNetworkRequest(HttpVerb verb, QString requestUrl,
                                         const QByteArray &data, PNetworkRetryPolicy *retryPolicy);
    QNetworkReply *executeNetworkRequest(HttpVerb verb, QString requestUrl,
                                         const QByteArray &data = QByteArray());

    ~PNetworkAccessManager();
signals:

public slots:

};

}
}

#endif // PNETWORKACCESSMANAGER_H
