#include "PNetworkAccessManager.h"

#include <QEventLoop>
#include <QTimer>
#include <QNetworkReply>

Pt::Network::PNetworkAccessManager::PNetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent), defaultRetryPolicy(PNetworkRetryPolicy::DefaultRetryPolicy())
{
}

void Pt::Network::PNetworkAccessManager::setRetryPolicy(Pt::Network::PNetworkRetryPolicy *retryPolicy)
{
    defaultRetryPolicy = retryPolicy;
}

QNetworkReply *Pt::Network::PNetworkAccessManager::executeNetworkRequest(HttpVerb verb,
                                                                         QString urlPattern,
                                                                         const std::map<QString, QString> &parameters,
                                                                         const QByteArray &data,
                                                                         PNetworkRetryPolicy *retryPolicy)
{

    for (auto parameter : parameters)
        urlPattern.replace(QString("{%1}").arg(parameter.first), parameter.second);

    qDebug() << urlPattern;
    QNetworkReply *reply;
    for (retryPolicy->initializeRetry(); retryPolicy->needToTry(); retryPolicy->moveNext()) {
        qDebug() << retryPolicy->timeout() << retryPolicy->needToTry();
        //qDebug() << QString("Tried %1 times").arg(i++);
        switch (verb) {
        case HttpVerb::Get:
            reply = get(QNetworkRequest(urlPattern));
            break;
        case HttpVerb::Post:
            reply = post(QNetworkRequest(urlPattern), data);
            break;
        case HttpVerb::Put:
            reply = put(QNetworkRequest(urlPattern), data);
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
        //connect(reply, &QNetworkReply::uploadProgress, [](int i){qDebug() << i;});
        // Temp code for Qt's old syntax.
        QTimer::singleShot(retryPolicy->timeout(), &loop, SLOT(quit()));
        loop.exec();
        if (retryPolicy->validate(reply))
            break;
        reply->deleteLater();
    }
    return reply;
}

QNetworkReply *Pt::Network::PNetworkAccessManager::executeNetworkRequest(Pt::Network::HttpVerb verb, QString urlPattern, const std::map<QString, QString> &parameters, const QByteArray &data)
{
    qDebug() << defaultRetryPolicy->timeout();
    return executeNetworkRequest(verb, urlPattern, parameters, data, defaultRetryPolicy->clone());
}

Pt::Network::PNetworkAccessManager::~PNetworkAccessManager()
{
    delete defaultRetryPolicy;
}
