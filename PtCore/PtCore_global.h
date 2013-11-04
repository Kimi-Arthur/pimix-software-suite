#ifndef PTCORE_GLOBAL_H
#define PTCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PTCORE_LIBRARY)
#  define PTCORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define PTCORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PTCORE_GLOBAL_H
