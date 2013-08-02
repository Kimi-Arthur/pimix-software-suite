#ifndef BASECAPRICORNWORKER_H
#define BASECAPRICORNWORKER_H

#include <QString>

class BaseCapricornWorker
{
public:
    typedef enum {
        Success, Failure
    } ResultType;
    virtual ResultType startJob(QJsonObject jobInfo) = 0;
    virtual ResultType stopJob() = 0;
    virtual ResultType pauseJob() = 0;
    virtual ResultType resumeJob() = 0;
    virtual int progressValue() = 0;
    virtual int progressMinimum() = 0;
    virtual int progressMaximum() = 0;
    virtual QString progressText() = 0;
};

#endif // BASECAPRICORNWORKER_H
