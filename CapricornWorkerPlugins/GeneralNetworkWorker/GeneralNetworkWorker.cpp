#include "GeneralNetworkWorker.h"


GeneralNetworkWorker::GeneralNetworkWorker()
{
}

BaseCapricornWorker::ResultType GeneralNetworkWorker::startJob(QJsonObject jobInfo)
{
    return Success;
}

BaseCapricornWorker::ResultType GeneralNetworkWorker::stopJob()
{
    return Success;
}

BaseCapricornWorker::ResultType GeneralNetworkWorker::pauseJob()
{
    return Success;
}

BaseCapricornWorker::ResultType GeneralNetworkWorker::resumeJob()
{
    return Success;
}

int GeneralNetworkWorker::progressValue()
{
    return 0;
}

int GeneralNetworkWorker::progressMinimum()
{
    return 0;
}

int GeneralNetworkWorker::progressMaximum()
{
    return 0;
}

QString GeneralNetworkWorker::progressText()
{
    // TODO
    return "";
}
