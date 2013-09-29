#include "PNetworkRetryPolicy.h"

Pt::Network::PNetworkRetryPolicy *Pt::Network::PNetworkRetryPolicy::DefaultRetryPolicy()
{
    return new PNetworkRetryPolicyFixedIntervalRetry(DefaultBaseTimeout, 1, Validator::NoErrorValidator);
}

Pt::Network::PNetworkRetryPolicy *Pt::Network::PNetworkRetryPolicy::NoRetryPolicy(int timeout, PNetworkValidator validator)
{
    return new PNetworkRetryPolicyFixedIntervalRetry(timeout, 1, validator);
}

Pt::Network::PNetworkRetryPolicy *Pt::Network::PNetworkRetryPolicy::FixedIntervalRetryPolicy(int timeout, int timesToTry, Pt::Network::PNetworkValidator validator)
{
    return new PNetworkRetryPolicyFixedIntervalRetry(timeout, timesToTry, validator);
}

Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::PNetworkRetryPolicyFixedIntervalRetry(int timeout, int timesToTry, Pt::Network::PNetworkValidator validator)
    : PNetworkRetryPolicy(validator), baseTimeout(timeout), timesToTry(timesToTry)
{
}

void Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::initializeRetry()
{
    timesTried = 0;
}

int Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::timeout()
{
    return baseTimeout;
}

void Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::moveNext()
{
    timesTried++;
}

bool Pt::Network::PNetworkRetryPolicyFixedIntervalRetry::needToTry()
{
    return timesTried < timesToTry;
}