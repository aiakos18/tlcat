#ifndef TLCAT_H
#define TLCAT_H

#include <QObject>
#include <QtNetwork/QUdpSocket>

class TLCat : public QObject
{
    Q_OBJECT
public:
    TLCat(const char *file = 0, unsigned int limit = 0);
    ~TLCat();

protected:
    void outputLog(const QString &log);

protected slots:
    void onReadyRead();

private:
    QUdpSocket      *m_udp;
    FILE            *m_file;
    unsigned int    m_limit;
    unsigned int    m_allOut;
};

#endif // TLCAT_H
