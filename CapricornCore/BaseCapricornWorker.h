#ifndef BASECAPRICORNWORKER_H
#define BASECAPRICORNWORKER_H

#include "capricorncore_global.h"
#include "CapricornWorker.h"
#include <QMap>

class CAPRICORNCORESHARED_EXPORT BaseCapricornWorker : public CapricornWorker
{
public:
    typedef ResultType (BaseCapricornWorker::*ExecuteMethod)(QJsonObject);

    QMap<QString, ExecuteMethod> methods;

    BaseCapricornWorker();
    virtual void run();
    virtual ResultType startJob(QJsonObject jobInfo=QJsonObject());
    virtual ResultType stopJob();
    virtual ResultType pauseJob();
    virtual ResultType resumeJob();
    virtual int progressValue();
    virtual int progressMinimum();
    virtual int progressMaximum();
    virtual QString progressText();
};

class CAPRICORNCORESHARED_EXPORT BaseCapricornWorkerFactory : public CapricornWorkerFactory
{
public:
    virtual CapricornWorker *createInstance();
    virtual void destroyInstance(CapricornWorker *workerInstance);
protected:
    QList<CapricornWorker *> workerInstances;
};

#endif // BASECAPRICORNWORKER_H
