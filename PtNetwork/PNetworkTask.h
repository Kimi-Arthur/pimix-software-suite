#ifndef PNETWORKTASK_H
#define PNETWORKTASK_H

#include "PtNetwork_global.h"
#include <QJsonObject>

class PTNETWORKSHARED_EXPORT PNetworkTask
{
public:
    virtual PNetworkTask(QJsonObject options=QJsonObject());
    virtual start();
};


#endif // PNETWORKTASK_H
