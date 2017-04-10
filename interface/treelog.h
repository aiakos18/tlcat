#ifndef TREELOG_H
#define TREELOG_H

#include <stdio.h>
#include <QString>
#include "tlcatif_global.h"

#if (defined(_WIN32) || defined(_WIN64))
#define __FUNCTION_NAME__ __FUNCTION__
#elif defined(__GNUC__) && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 1)
#define __FUNCTION_NAME__ __PRETTY_FUNCTION__
#else
#error treelog can only be supported on wins and unix now
#endif

class TLCATIFSHARED_EXPORT TreeTrace
{
public:
    TreeTrace(const char *tag, const char *func);
    ~TreeTrace();
};

void TLCATIFSHARED_EXPORT nmllog(const char *level, const char *tag, const char *function, const QString &s);
void TLCATIFSHARED_EXPORT treelog(const char *level, const char *tag, const QString &s);


#define NMLLOG(level, tag, format, ...) nmllog(level, tag, __FUNCTION_NAME__, QString().sprintf(format, ##__VA_ARGS__))

#define TREETRACE(tag) TreeTrace __for_tree_trace__(tag, __FUNCTION_NAME__)
#define TREELOG(level, tag, format, ...) treelog(level, tag, QString().sprintf(format, ##__VA_ARGS__))

#endif // TREELOG_H
