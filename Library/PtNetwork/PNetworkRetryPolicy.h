#ifndef PNETWORKRETRYPOLICY_H
#define PNETWORKRETRYPOLICY_H

#include <QNetworkReply>
#include "PtNetwork_global.h"
#include <QDebug>

namespace Pt {
namespace Network {


typedef std::function<bool(QNetworkReply *)> PNetworkValidator;

namespace Validator {

const PNetworkValidator NoErrorValidator = [](QNetworkReply *reply) { return reply->isFinished() && reply->error() == QNetworkReply::NoError; };
//const PNetworkValidator ContentRegexValidator = [](QNetworkReply *reply) { return reply->isFinished() && reply->error() == QNetworkReply::NoError; };

}

class PTNETWORKSHARED_EXPORT PNetworkRetryPolicy
{
    PNetworkValidator validator;
protected:
    bool status;
public:
    static const int DefaultBaseTimeout = 300000;
    static PNetworkRetryPolicy *DefaultRetryPolicy();
    static PNetworkRetryPolicy *NoRetryPolicy(int timeout, PNetworkValidator validator = Validator::NoErrorValidator);
    static PNetworkRetryPolicy *LimitedRetryPolicy(int timeout, int timesToTry, PNetworkValidator validator = Validator::NoErrorValidator);
    static PNetworkRetryPolicy *UnlimitedRetryPolicy(int timeout, PNetworkValidator validator = Validator::NoErrorValidator);
    PNetworkRetryPolicy(PNetworkValidator validator) : validator(validator) { status = false; }
    PNetworkRetryPolicy(const PNetworkRetryPolicy &other) : validator(other.validator), status(other.status) {}
    // Keep sync with QTimer::start(int msec)
    virtual int timeout() { return 0; }
    virtual PNetworkRetryPolicy *clone() { return new PNetworkRetryPolicy(*this); }
    virtual void initializeRetry() {}
    virtual void moveNext() {}
    virtual bool validate(QNetworkReply *reply) { return validator(reply); }
    virtual bool needToTry() { return false; }
    bool isSuccessful() { return status; }
    virtual ~PNetworkRetryPolicy() {}
};

class PNetworkRetryPolicyFixedIntervalRetry : public PNetworkRetryPolicy
{
    int baseTimeout;
    int timesToTry, timesTried;
public:
    PNetworkRetryPolicyFixedIntervalRetry(const PNetworkRetryPolicyFixedIntervalRetry &other);
    PNetworkRetryPolicyFixedIntervalRetry(int timeout, int timesToTry, PNetworkValidator validator);
    virtual void initializeRetry();
    virtual PNetworkRetryPolicy *clone();
    virtual int timeout();
    virtual void moveNext();
    virtual bool needToTry();
};

}
}

#endif // PNETWORKRETRYPOLICY_H
