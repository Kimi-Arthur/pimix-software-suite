#include "PNetworkRetryPolicy.h"
#include <QThread>

Pt::Network::PNetworkRetryPolicy *Pt::Network::PNetworkRetryPolicy::DefaultRetryPolicy()
{
    return new PNetworkRetryPolicyFixedIntervalRetry(1, DefaultBaseTimeout, 0, Validator::NoErrorValidator);
}

Pt::Network::PNetworkRetryPolicy *Pt::Network::PNetworkRetryPolicy::NoRetryPolicy(int timeout, int wait, PNetworkValidator validator)
{
    return new PNetworkRetryPolicyFixedIntervalRetry(1, timeout, wait, validator);
}

Pt::Network::PNetworkRetryPolicy *Pt::Network::PNetworkRetryPolicy::LimitedRetryPolicy(int timesToTry, int timeout, int wait, Pt::Network::PNetworkValidator validator)
{
    return new PNetworkRetryPolicyFixedIntervalRetry(timesToTry, timeout, wait, validator);
}

Pt::Network::PNetworkRetryPolicy *Pt::Network::PNetworkRetryPolicy::UnlimitedRetryPolicy(int timeout, int wait, Pt::Network::PNetworkValidator validator)
{
    return new PNetworkRetryPolicyFixedIntervalRetry(-1, timeout, wait, validator);
}

Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::PNetworkRetryPolicyFixedIntervalRetry(int timesToTry, int timeout, int waitTime, Pt::Network::PNetworkValidator validator)
    : PNetworkRetryPolicy(validator), baseTimeout(timeout), timesToTry(timesToTry), waitTime(waitTime)
{
}

void Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::initializeRetry()
{
    timesTried = 0;
}

Pt::Network::PNetworkRetryPolicy *Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::clone()
{
    return new PNetworkRetryPolicyFixedIntervalRetry(*this);
}

int Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::timeout()
{
    return baseTimeout;
}

void Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::moveNext()
{
    timesTried++;
    QThread::msleep(waitTime);
}

bool Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::needToTry()
{
    return timesToTry < 0 || timesTried < timesToTry;
}


Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::PNetworkRetryPolicyFixedIntervalRetry(const Pt::Network::PNetworkRetryPolicyFixedIntervalRetry &other)
    : PNetworkRetryPolicy(other), baseTimeout(other.baseTimeout), timesToTry(other.timesToTry), timesTried(other.timesTried), waitTime(other.waitTime)
{
}
