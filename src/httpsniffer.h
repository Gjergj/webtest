#ifndef HTTPSNIFFER_H
#define HTTPSNIFFER_H

#include <QObject>
#include <QtDebug>
#include <QDateTime>

#include <ctype.h>
#include <pcap.h>
#include "tcp.h"
#include <string.h>
#include <time.h>
#include <cstdio>
#include <stdlib.h>
#include "interfaceinfo.h"
#include "sniffeddata.h"

#ifdef __linux
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <signal.h>
#include <sys/stat.h>


#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#endif //#ifdef __linux

#ifdef _WIN32
#include <WS2tcpip.h>
#endif //_WIN32

#include <string>
#include <iostream>
#include <stdexcept>
#include <boost/regex.hpp>
//#include "tins/tcp_ip/stream_follower.h"
//#include "tins/sniffer.h"

class httpsniffer : public QObject
{
    Q_OBJECT
public:
    explicit httpsniffer(QObject *parent = 0);
    ~httpsniffer();
    QList<InterfaceInfo> enumerateInterfaces();

    std::string ethInterface() const;
    void setEthInterface(const std::string &ethInterface);

    std::string filter() const;
    void setFilter(const std::string &filter);

public slots:
    void startSniff();
    void stop();
signals:
    void receivedData(SniffedData data);
    void error(QString err);
    void finished();
private:

    void set_link_offset(int header_type);

    pcap_t *pcap_hnd = NULL;
    int set_promisc = 1;
    char *use_infile = NULL;
    pcap_t *prepare_capture(char *ethInterface, int promisc, char *filename, char *capfilter);

    std::string m_ethInterface;
    std::string m_filter;

};

#endif // HTTPSNIFFER_H
