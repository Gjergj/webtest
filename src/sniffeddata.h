#ifndef SNIFFEDDATA_H
#define SNIFFEDDATA_H
#include <QByteArray>
#include <QString>
struct SniffedData{
    QString sourceIp;
    QString destinationIp;
    QString sourcePort;
    QString destinationPort;
    QString timestamp;
    QByteArray data;
    qint64 packetReceivedTime;
    uint64_t packetSize;
    uint64_t seqNo;
};

#endif // SNIFFEDDATA_H
