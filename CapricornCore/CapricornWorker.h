#ifndef CAPRICORNWORKER_H
#define CAPRICORNWORKER_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include "capricorncore_global.h"


class CAPRICORNCORESHARED_EXPORT CapricornWorker
{
public:
    typedef enum {
        Success, Failure
    } ResultType;
    virtual ResultType startJob(QJsonObject jobInfo=QJsonObject()) = 0;
    virtual ResultType stopJob() = 0;
    virtual ResultType pauseJob() = 0;
    virtual ResultType resumeJob() = 0;
    virtual int progressValue() = 0;
    virtual int progressMinimum() = 0;
    virtual int progressMaximum() = 0;
    virtual QString progressText() = 0;
};


#define CapricornWorker_iid "org.Pimix.Capricorn.Worker"

Q_DECLARE_INTERFACE(CapricornWorker, CapricornWorker_iid)



class CAPRICORNCORESHARED_EXPORT CapricornWorkerFactory
{
public:
    virtual CapricornWorker *createInstance() = 0;
    virtual void destroyInstance(CapricornWorker *workerInstance) = 0;
};

#define CapricornWorkerFactory_iid "org.Pimix.Capricorn.WorkerFactory"

Q_DECLARE_INTERFACE(CapricornWorkerFactory, CapricornWorkerFactory_iid)

#endif // CAPRICORNWORKER_H
