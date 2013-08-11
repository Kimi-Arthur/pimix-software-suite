#ifndef GENERALNETWORKWORKER_H
#define GENERALNETWORKWORKER_H

#include "generalnetworkworker_global.h"
#include "BaseCapricornWorker.h"
#include <QObject>

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
};

#endif // GENERALNETWORKWORKER_H
