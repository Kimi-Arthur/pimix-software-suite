#ifndef PTGUI_GLOBAL_H
#define PTGUI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PTGUI_LIBRARY)
#  define PTGUISHARED_EXPORT Q_DECL_EXPORT
#else
#  define PTGUISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PTGUI_GLOBAL_H
