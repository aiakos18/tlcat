#include <stdio.h>
#include <QCoreApplication>

#include "tlcat.h"

int main(int argc, char *argv[])
{
    int limitSize = 0;
    const char *file = 0;
    for (int i = 1; i < argc; ++i) {
        char *arg = argv[i];
        if (strstr(arg, "limit=") == arg) {
            char *limitStr = arg + strlen("limit=");
            limitSize = atoi(limitStr);
        }
        if (strstr(arg, "out=") == arg) {
            file = arg + strlen("out=");
        }
    }

    printf("file: %s, limit size: %d\n", file ? file : "NULL", limitSize);

    QCoreApplication a(argc, argv);
    TLCat c(file, limitSize);

    return a.exec();
}
