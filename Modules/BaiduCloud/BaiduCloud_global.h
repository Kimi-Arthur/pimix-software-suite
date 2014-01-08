#ifndef BAIDUCLOUD_GLOBAL_H
#define BAIDUCLOUD_GLOBAL_H

#include <QtCore/qglobal.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

#include "PFile.h"
#include "PtCore.h"
#include "PtNetwork.h"
#include "PJsonValue.h"
#include "PLogger.h"
#include "PString.h"
#include <map>
#include "PNetworkAccessManager.h"


#if defined(BAIDUCLOUD_LIBRARY)
#  define BAIDUCLOUDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BAIDUCLOUDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BAIDUCLOUD_GLOBAL_H
