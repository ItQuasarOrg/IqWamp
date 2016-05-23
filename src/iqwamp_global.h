#ifndef IQWAMP_GLOBAL_H
#define IQWAMP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(IQWAMP_LIBRARY)
#  define IQWAMPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define IQWAMPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // IQWAMP_GLOBAL_H
