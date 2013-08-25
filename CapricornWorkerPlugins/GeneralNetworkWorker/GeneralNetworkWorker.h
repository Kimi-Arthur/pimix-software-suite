#ifndef GENERALNETWORKWORKER_H
#define GENERALNETWORKWORKER_H

#include "generalnetworkworker_global.h"
#include "BaseCapricornWorker.h"
#include <QObject>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


#define GeneralNetworkWorker_iid "org.Pimix.Capricorn.Worker.GeneralNetwork"

class GENERALNETWORKWORKERSHARED_EXPORT GeneralNetworkWorker : public QObject, public BaseCapricornWorker
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID GeneralNetworkWorker_iid)
    Q_INTERFACES(CapricornWorker)
public:
    GeneralNetworkWorker();
    ResultType download(QJsonObject jobInfo);
    QString text;
    int timeout;
    int total, count;
    QNetworkAccessManager m;
public slots:
    void receivePayload(QNetworkReply *reply);
};

class GENERALNETWORKWORKERSHARED_EXPORT GeneralNetworkWorkerFactory : public QObject, public BaseCapricornWorkerFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID GeneralNetworkWorker_iid)
    Q_INTERFACES(CapricornWorkerFactory)
public:
    CapricornWorker *createInstance();

};

#endif // GENERALNETWORKWORKER_H
