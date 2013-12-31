#ifndef PTCORE_GLOBAL_H
#define PTCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#include <QByteArray>
#include <functional>
#include <QStringList>
#include <QString>
#include <QMap>
#include <QSet>
#include <QDateTime>
#include <QTextStream>
#include <map>

#define QSL QStringLiteral

#if defined(PTCORE_LIBRARY)
#  define PTCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define PTCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PTCORE_GLOBAL_H
