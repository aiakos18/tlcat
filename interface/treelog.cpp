#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#if (defined(_WIN32) || defined(_WIN64))
#include <windows.h>
#elif defined(__GNUC__) && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 1)
#include <sys/syscall.h>
#endif

#include "treelog.h"
#include "queue.h"

static pthread_key_t g_spacesKey = 0;
static pthread_once_t g_once = PTHREAD_ONCE_INIT;

#include <QtNetwork/QUdpSocket>
static void sendLogToServer(const QString &log)
{
    static QUdpSocket *udp = new QUdpSocket();
    if (udp->writeDatagram(qPrintable(log), QHostAddress("127.0.0.1"), 2016) == -1) {
        qDebug("udp writeDatagram error: %s", qPrintable(udp->errorString()));
    }
}

static void once_create_keys()
{
    pthread_key_create(&g_spacesKey, NULL);
}

static int get_spaces_num()
{
    pthread_once(&g_once, once_create_keys);

    int *spacesNum = (int*)pthread_getspecific(g_spacesKey);
    if (spacesNum == NULL) {
        spacesNum = (int*)malloc(sizeof(int));
        pthread_setspecific(g_spacesKey, spacesNum);
        *spacesNum = 0;
    }
    return *spacesNum;
}

static void set_spaces_num(int num)
{
    int *spacesNum = (int*)pthread_getspecific(g_spacesKey);
    if (spacesNum == NULL) return;
    *spacesNum = num;
}

void nmllog(const char *level, const char *tag, const char *function, const QString &s)
{
    if (!level || !tag || !function) {
        return;
    }

    timeval tv;
    gettimeofday(&tv, NULL);
    time_t tt = time(NULL);
    tm *lt = localtime(&tt);

    QString log;
    log.sprintf("[%02d:%02d:%02d.%06ld][%lu] [%s][%s] %s: %s",
                lt->tm_hour, lt->tm_min, lt->tm_sec, tv.tv_usec,
            #if (defined(_WIN32) || defined(_WIN64))
                GetCurrentThreadId(),
            #elif defined(__GNUC__) && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 1)
                syscall(SYS_gettid),
            #endif
                level, tag, function, qPrintable(s));
//    Queue::inst()->addQueue(log);
    sendLogToServer(log);
}

void treelog(const char *level, const char *tag, const QString &s)
{
    if (!level || !tag) {
        return;
    }

    timeval tv;
    gettimeofday(&tv, NULL);
    time_t tt = time(NULL);
    tm *lt = localtime(&tt);

    QString log;
    int spacesNum = get_spaces_num();
    spacesNum += strlen(tag) > 8 ? 0 : (8 - strlen(tag));
    if (spacesNum < 0) spacesNum = 0;
    log.sprintf("[%02d:%02d:%02d.%06ld][%lu] %s[%s] %s%s",
                lt->tm_hour, lt->tm_min, lt->tm_sec, tv.tv_usec,
            #if (defined(_WIN32) || defined(_WIN64))
                GetCurrentThreadId(),
            #elif defined(__GNUC__) && (__GNUC__ > 4 || __GNUC__ == 4 && __GNUC_MINOR__ >= 1)
                syscall(SYS_gettid),
            #endif
                strcmp("TREE", level) ? QString("[%1]").arg(level).toStdString().c_str() : "      ",
                tag, std::string(spacesNum, ' ').c_str(), qPrintable(s));
//    Queue::inst()->addQueue(log);
    sendLogToServer(log);
}

TreeTrace::TreeTrace(const char *tag, const char *func)
{
    QString s;
    s.sprintf(">>> %s", func ? func : "");
    treelog("TREE", tag ? tag : "", s);
    set_spaces_num(get_spaces_num() + 4);
}

TreeTrace::~TreeTrace()
{
    set_spaces_num(get_spaces_num() - 4);
}

