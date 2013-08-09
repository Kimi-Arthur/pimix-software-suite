#ifndef PTNETWORK_GLOBAL_H
#define PTNETWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PTNETWORK_LIBRARY)
#  define PTNETWORKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PTNETWORKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PTNETWORK_GLOBAL_H
