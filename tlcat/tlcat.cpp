#include <stdio.h>
#include <fcntl.h>
#include "tlcat.h"


TLCat::TLCat(const char *file, unsigned int limit)
    : m_udp(0)
    , m_file(0)
    , m_limit(limit * 1024 * 1024)
    , m_allOut(0)
{
    m_udp = new QUdpSocket();
    m_udp->bind(QHostAddress("127.0.0.1"), 2016);
    connect(m_udp, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    if (file) {
        m_file = fopen(file, "w");
        if (!m_file) {
            printf("cannot create file: %s\n", file);
        }
    }
}

TLCat::~TLCat()
{
    if (m_file) {
        fclose(m_file);
        m_file = 0;
    }
    if (m_udp) {
        delete m_udp;
        m_udp = 0;
    }
}

void TLCat::outputLog(const QString &log)
{
    std::string out = (log + QString("\n")).toStdString();
    if (m_file) {
        if (m_allOut + out.size() < m_limit) {
            fwrite(out.c_str(), out.size(), 1, m_file);
            m_allOut += out.size();
            fflush(m_file);
        }
        else {
            fclose(m_file);
            m_file = 0;
        }
    }
    else {
        printf("%s", out.c_str());
        fflush(stdout);
    }
}

void TLCat::onReadyRead()
{
    while (m_udp->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(m_udp->pendingDatagramSize());
        m_udp->readDatagram(data.data(), data.size());
        outputLog(QString(data.data()));
    }
}
