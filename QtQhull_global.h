#ifndef QTQHULL_GLOBAL_H
#define QTQHULL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTQHULL_LIBRARY)
#  define QTQHULL_EXPORT Q_DECL_EXPORT
#else
#  define QTQHULL_EXPORT Q_DECL_IMPORT
#endif

#endif // QTQHULL_GLOBAL_H
