#include "PNetworkAccessManager.h"

#include <QEventLoop>
#include <QTimer>
#include <QNetworkReply>

Pt::Network::PNetworkAccessManager::PNetworkAccessManager(Pt::Core::PLogger *logger, QObject *parent)
    : QNetworkAccessManager(parent),
      defaultRetryPolicy(PNetworkRetryPolicy::DefaultRetryPolicy()),
      logger(logger)
{
}

void Pt::Network::PNetworkAccessManager::setRetryPolicy(Pt::Network::PNetworkRetryPolicy *retryPolicy)
{
    defaultRetryPolicy = retryPolicy;
}

QNetworkReply *Pt::Network::PNetworkAccessManager::executeNetworkRequest(Pt::Network::HttpVerb verb, QNetworkRequest request, const QByteArray &data, Pt::Network::PNetworkRetryPolicy *retryPolicy)
{
    logger->logMethodIn(__PFUNC_ID__);
    logger->debug(request.url(), "Request url");
    QNetworkReply *reply = nullptr;
    int i = 0;
    for (retryPolicy->initializeRetry(); retryPolicy->needToTry(); retryPolicy->moveNext()) {
        // Clear reply from start other than finish to preserve failure status
        if (reply != nullptr)
            reply->deleteLater();

        logger->debug(QString("Started %1 try:").arg(++i));

        switch (verb) {
        case HttpVerb::Get:
            reply = get(request);
            break;
        case HttpVerb::Post:
            reply = post(request, data);
            break;
        case HttpVerb::Put:
            reply = put(request, data);
            break;
        case HttpVerb::Head:
        case HttpVerb::Delete:
        case HttpVerb::Trace:
        case HttpVerb::Options:
        case HttpVerb::Connect:
        case HttpVerb::Patch:
        default:
            break;
        }
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        // Temp code for Qt's old syntax.
        QTimer::singleShot(retryPolicy->timeout(), &loop, SLOT(quit()));
        loop.exec();
        if (retryPolicy->validate(reply))
        {
            logger->debug(QString("Succeeded %1 try.").arg(i));
            break;
        }

        logger->debug(QString("Failed %1 try.").arg(i));
    }
    logger->logMethodOut(__PFUNC_ID__);
    return reply;
}

QNetworkReply *Pt::Network::PNetworkAccessManager::executeNetworkRequest(Pt::Network::HttpVerb verb, QNetworkRequest request, const QByteArray &data)
{
    return executeNetworkRequest(verb, request, data, defaultRetryPolicy->clone());
}

QNetworkReply *Pt::Network::PNetworkAccessManager::executeNetworkRequest(HttpVerb verb,
                                                                         QString requestUrl,
                                                                         const QByteArray &data,
                                                                         PNetworkRetryPolicy *retryPolicy)
{
    // Add default value for requests with only urls
    QNetworkRequest request(requestUrl);
    switch (verb) {
    case HttpVerb::Post:
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        break;
    case HttpVerb::Get:
    case HttpVerb::Put:
    case HttpVerb::Head:
    case HttpVerb::Delete:
    case HttpVerb::Trace:
    case HttpVerb::Options:
    case HttpVerb::Connect:
    case HttpVerb::Patch:
    default:
        break;
    }
    return executeNetworkRequest(verb, request, data, retryPolicy);
}

QNetworkReply *Pt::Network::PNetworkAccessManager::executeNetworkRequest(Pt::Network::HttpVerb verb, QString requestUrl, const QByteArray &data)
{
    return executeNetworkRequest(verb, requestUrl, data, defaultRetryPolicy->clone());
}

Pt::Network::PNetworkAccessManager::~PNetworkAccessManager()
{
    delete defaultRetryPolicy;
}
