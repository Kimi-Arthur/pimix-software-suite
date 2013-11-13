#ifndef PTLOGGING_GLOBAL_H
#define PTLOGGING_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PTLOGGING_LIBRARY)
#  define PTLOGGINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PTLOGGINGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PTLOGGING_GLOBAL_H
