#ifndef QUEUE_H
#define QUEUE_H

#include <QObject>
#include <QThread>

class Queue : public QObject
{
    Q_OBJECT
protected:
    explicit Queue(QObject *parent = 0);
    ~Queue();

public:
    static Queue *inst();

    void addQueue(const QString &s);

protected:
    Q_INVOKABLE void slot_addQueue(const QString &s);

private:
    static Queue *s_inst;
    QThread m_thread;
};

#endif // QUEUE_H
