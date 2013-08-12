#ifndef GENERALNETWORKWORKER_H
#define GENERALNETWORKWORKER_H

#include "generalnetworkworker_global.h"
#include "BaseCapricornWorker.h"
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


#define GeneralNetworkWorker_iid "org.Pimix.Capricorn.Worker.GeneralNetwork"

class GENERALNETWORKWORKERSHARED_EXPORT GeneralNetworkWorker : public QObject, public BaseCapricornWorker
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID GeneralNetworkWorker_iid)
    Q_INTERFACES(CapricornWorker)
public:
    GeneralNetworkWorker();
    ResultType download(QJsonObject jobInfo);
    static QString text;
    int timeout;
    int total, count;
public slots:
    void receivePayload(QNetworkReply *reply);
};

#endif // GENERALNETWORKWORKER_H
