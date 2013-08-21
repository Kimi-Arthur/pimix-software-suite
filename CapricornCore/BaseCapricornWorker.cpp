#include "BaseCapricornWorker.h"
#include <QtConcurrent/QtConcurrent>

BaseCapricornWorker::BaseCapricornWorker()
{
}

CapricornWorker::ResultType BaseCapricornWorker::startJob(QJsonObject jobInfo)
{
    qDebug() << jobInfo;
    QString method = jobInfo["method"].toString();
    QtConcurrent::run(this, methods[method], jobInfo);
    return Success;
}

CapricornWorker::ResultType BaseCapricornWorker::stopJob()
{
    return Success;
}

CapricornWorker::ResultType BaseCapricornWorker::pauseJob()
{
    return Success;
}

CapricornWorker::ResultType BaseCapricornWorker::resumeJob()
{
    return Success;
}

int BaseCapricornWorker::progressValue()
{
    return Success;
}

int BaseCapricornWorker::progressMinimum()
{
    return Success;
}

int BaseCapricornWorker::progressMaximum()
{
    return Success;
}

QString BaseCapricornWorker::progressText()
{
    return "";
}


CapricornWorker *BaseCapricornWorkerFactory::createInstance()
{
    qDebug() << "You need to implement createInstance method!";
    return nullptr;
}

void BaseCapricornWorkerFactory::destroyInstance(CapricornWorker *workerInstance)
{
    Q_ASSERT(workerInstances.removeAll(workerInstance) == 1);
}
