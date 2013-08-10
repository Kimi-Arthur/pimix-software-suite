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
    virtual ResultType startJob(QJsonObject jobInfo);
    virtual ResultType stopJob();
    virtual ResultType pauseJob();
    virtual ResultType resumeJob();
    virtual int progressValue();
    virtual int progressMinimum();
    virtual int progressMaximum();
    virtual QString progressText();
};

#endif // GENERALNETWORKWORKER_H
