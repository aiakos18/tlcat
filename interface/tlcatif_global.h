#ifndef TLCATIF_GLOBAL_H
#define TLCATIF_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TLCATIF_LIBRARY)
#  define TLCATIFSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TLCATIFSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TLCATIF_GLOBAL_H
