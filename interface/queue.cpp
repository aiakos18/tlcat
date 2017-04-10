#include <QtNetwork/QUdpSocket>
#include "queue.h"

static void sendLogToServer(const QString &log)
{
    static QUdpSocket *udp = new QUdpSocket();
    if (udp->writeDatagram(qPrintable(log), QHostAddress("127.0.0.1"), 2016) == -1) {
        qDebug("udp writeDatagram error: %s", qPrintable(udp->errorString()));
    }
}

Queue* Queue::s_inst = 0;

Queue::Queue(QObject *parent) : QObject(parent)
{
    moveToThread(&m_thread);
}

Queue::~Queue()
{

}

Queue *Queue::inst()
{
    if (!s_inst) {
        s_inst = new Queue();
        s_inst->m_thread.start();
    }
    return s_inst;
}

void Queue::addQueue(const QString &s)
{
    QMetaObject::invokeMethod(this, "slot_addQueue", Qt::QueuedConnection, Q_ARG(QString,s));
}

void Queue::slot_addQueue(const QString &s)
{
    sendLogToServer(s);
}

