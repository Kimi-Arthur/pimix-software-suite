#ifndef GENERALNETWORKWORKER_H
#define GENERALNETWORKWORKER_H

#include "generalnetworkworker_global.h"
#include "BaseCapricornWorker.h"

class GENERALNETWORKWORKERSHARED_EXPORT GeneralNetworkWorker : public BaseCapricornWorker
{
    
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
