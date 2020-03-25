#include "httpsniffer.h"
#define PORTSTRLEN 6
#define MAX_TIME_LEN 32

int link_offset = 0;
int eth_skip_bits = 0;
httpsniffer::httpsniffer(QObject *parent) : QObject(parent)
{

}

httpsniffer::~httpsniffer()
{
}

/* Iterate through IPv6 extension headers looking for a TCP header. Returns
   the total size of the IPv6 header, including all extension headers.
   Return 0 to abort processing of this packet. */
int process_ip6_nh(const u_char *pkt, int size_ip, unsigned int caplen, unsigned int offset) {
    const struct ip6_ext_header *ip6_eh;
    unsigned int len = caplen - offset;
    ip6_eh = (struct ip6_ext_header *) (pkt + offset + size_ip);

    while (ip6_eh->ip6_eh_nh != IPPROTO_TCP) {
        switch (ip6_eh->ip6_eh_nh) {
        case 0:  /* Hop-by-hop options */
        case 43: /* Routing */
        case 44: /* Fragment */
        case 51: /* Authentication Header */
        case 50: /* Encapsulating Security Payload */
        case 60: /* Destination Options */
            size_ip = size_ip + (ip6_eh->ip6_eh_len * 8) + 8;
            break;
        case 59: /* No next header */
        default:
            return 0;
        }

        if (size_ip > len) return 0;

        ip6_eh = (struct ip6_ext_header *) (pkt + offset + size_ip);
    }

    /* Next header is TCP, so increment past the final extension header */
    size_ip = size_ip + (ip6_eh->ip6_eh_len * 8) + 8;

    return size_ip;
}

/* Process each packet that passes the capture filter */
void parse_http_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *pkt) {
    httpsniffer *sniff = reinterpret_cast<httpsniffer*>(args);
    struct tm *pkt_time;
    char *header_line, *req_value;
    char saddr[INET6_ADDRSTRLEN], daddr[INET6_ADDRSTRLEN];
    char sport[PORTSTRLEN], dport[PORTSTRLEN];
    char ts[MAX_TIME_LEN], fmt[MAX_TIME_LEN];

    int is_request = 0, is_response = 0;
    unsigned int eth_type = 0, offset;

    const struct eth_header *eth;
    const struct ip_header *ip;
    const struct ip6_headers *ip6;
    const struct tcp_header *tcp;
    const char *data;
    int size_ip, size_tcp, size_data, family;

    /* Check the ethernet type and insert a VLAN offset if necessary */
    eth = (struct eth_header *) pkt;
    eth_type = ntohs(eth->ether_type);
    if (eth_type == ETHER_TYPE_VLAN) {
        offset = link_offset + 4;
    } else {
        offset = link_offset;
    }

    offset += eth_skip_bits;

    /* Position pointers within packet stream and do sanity checks */
    ip = (struct ip_header *) (pkt + offset);
    ip6 = (struct ip6_headers *) (pkt + offset);

    switch (IP_V(ip)) {
    case 4: family = AF_INET; break;
    case 6: family = AF_INET6; break;
    default: return;
    }

    if (family == AF_INET) {
        size_ip = IP_HL(ip) * 4;
        if (size_ip < 20) return;
        if (ip->ip_p != IPPROTO_TCP) return;
    } else { /* AF_INET6 */
        size_ip = sizeof(struct ip6_headers);
        if (ip6->ip6_nhs != IPPROTO_TCP)
            size_ip = process_ip6_nh(pkt, size_ip, header->caplen, offset);
        if (size_ip < 40) return;
    }

    tcp = (struct tcp_header *) (pkt + offset + size_ip);
    size_tcp = TH_OFF(tcp) * 4;
    if (size_tcp < 20) return;

    data = (char *) (pkt + offset + size_ip + size_tcp);
    size_data = (header->caplen - (offset + size_ip + size_tcp));
    if (size_data <= 0) return;

    /* Grab source/destination IP addresses */
    if (family == AF_INET) {
        //inet_ntop(family, &ip->ip_src, saddr, sizeof(saddr));
        inet_ntop(family, (void *)&ip->ip_src, saddr, sizeof(saddr));
        inet_ntop(family, (void *)&ip->ip_dst, daddr, sizeof(daddr));
    } else { /* AF_INET6 */
        inet_ntop(family, (void *)&ip6->ip_srcs, saddr, sizeof(saddr));
        inet_ntop(family, (void *)&ip6->ip_dsts, daddr, sizeof(daddr));
    }
    SniffedData receivedData;
    receivedData.seqNo = tcp->th_seq;
    receivedData.sourceIp = QString(saddr);
    receivedData.destinationIp = QString(daddr);

    /* Grab source/destination ports */
#ifdef _WIN32
    _snprintf_s(sport, PORTSTRLEN, "%d", ntohs(tcp->th_sport));
    _snprintf_s(dport, PORTSTRLEN, "%d", ntohs(tcp->th_dport));
#else
    snprintf(sport, PORTSTRLEN, "%d", ntohs(tcp->th_sport));
    snprintf(dport, PORTSTRLEN, "%d", ntohs(tcp->th_dport));
#endif
    receivedData.sourcePort = QString(sport);
    receivedData.destinationPort = QString(dport);
#ifdef _WIN32
    _snprintf_s(ts, MAX_TIME_LEN, "%s", ctime((const time_t*)&header->ts.tv_sec));
#else
    /* Extract packet capture time */
    pkt_time = localtime((time_t *) &header->ts.tv_sec);
    strftime(fmt, sizeof(fmt), "%Y-%m-%d %H:%M:%S.%%03u", pkt_time);
    snprintf(ts, sizeof(ts), fmt, header->ts.tv_usec / 1000);
#endif

    receivedData.timestamp = QString(ts);
    receivedData.packetSize = size_data;
    receivedData.packetReceivedTime = QDateTime::currentMSecsSinceEpoch();
    receivedData.data.append(data,size_data);
    emit sniff->receivedData(receivedData);

    return;
}

void httpsniffer::startSniff(){
    if(m_ethInterface.empty()){
        emit error("No network interface selected.");
        return;
    }
    if(m_filter.empty()){
        emit error("Set port number.");
        return;
    }
    int loop_status;
    pcap_hnd = prepare_capture((char*)m_ethInterface.c_str(), set_promisc, use_infile, (char*)m_filter.c_str());

    if(pcap_hnd == NULL){
        emit error("Problem reading packets from this interface. Did you run as root?");
        return;
    }
    loop_status = pcap_loop(pcap_hnd, -1, parse_http_packet, reinterpret_cast<u_char*>(this));
    if (loop_status == -1) {
        emit error("Problem reading packets from interface: " + QString(pcap_geterr(pcap_hnd)));
    } else if (loop_status == -2) {
        emit error("Loop halted, shutting down...");
    }
}
/* Perform end of run tasks and prepare to exit gracefully */
void httpsniffer::stop() {
    /* This may have already been called, but might not
           have depending on how we got here */
    if (pcap_hnd)
        pcap_breakloop(pcap_hnd);
    fflush(NULL);

    if (pcap_hnd)
        pcap_close(pcap_hnd);
    emit finished();
    return;
}

std::string httpsniffer::filter() const
{
    return m_filter;
}

void httpsniffer::setFilter(const std::string &filter)
{
    m_filter = filter;
}

std::string httpsniffer::ethInterface() const
{
    return m_ethInterface;
}

void httpsniffer::setEthInterface(const std::string &ethInterface)
{
    m_ethInterface = ethInterface;
}

char* ip6tos(struct sockaddr *sockaddr, char *address, int addrlen)
{
    socklen_t sockaddrlen;

#ifdef WIN32
    sockaddrlen = sizeof(struct sockaddr_in6);
#else
    sockaddrlen = sizeof(struct sockaddr_storage);
#endif


    if(getnameinfo(sockaddr,
                   sockaddrlen,
                   address,
                   addrlen,
                   NULL,
                   0,
                   NI_NUMERICHOST) != 0) address = NULL;

    return address;
}

/* From tcptraceroute, convert a numeric IP address to a string */
#define IPTOSBUFFERS	12
char *iptos(u_long in)
{
    static char output[IPTOSBUFFERS][3*4+3+1];
    static short which;
    u_char *p;

    p = (u_char *)&in;
    which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
#ifdef _WIN32
    _snprintf_s(output[which], sizeof(output[which]), sizeof(output[which]),"%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
#else
   snprintf(output[which], sizeof(output[which]),"%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
#endif
    return output[which];
}

QList<InterfaceInfo> httpsniffer::enumerateInterfaces(){
    pcap_if_t *devList = NULL;
    char ip6str[128];
    char errbuf[PCAP_ERRBUF_SIZE];
    QList<InterfaceInfo> interfaces;
    if (pcap_findalldevs(&devList, errbuf) == -1)
    {
        emit error("There is a problem enumerating interfaces " + QString(errbuf));
        return interfaces;
    }
    pcap_if_t *dev;

    if(devList){
        /* scan the list for a suitable device to capture from */
        for (dev = devList; dev != NULL; dev = dev->next)
        {
            InterfaceInfo iface;
            pcap_addr_t *dev_addr; //interface address that used by pcap_findalldevs()
            bool ifaceOk = false;
            /* check if the device captureble*/
            for(dev_addr = dev->addresses; dev_addr; dev_addr = dev_addr->next){
                switch(dev_addr->addr->sa_family)
                {
                case AF_INET:
                    //printf("\tAddress Family Name: AF_INET\n");
                    if (dev_addr->addr){
                        iface.address = QString(iptos(((struct sockaddr_in *)dev_addr->addr)->sin_addr.s_addr));
                        //printf("\tAddress: %s\n",iptos(((struct sockaddr_in *)dev_addr->addr)->sin_addr.s_addr));
                    }
                    if (dev_addr->netmask){
                        iface.netmask = QString(iptos(((struct sockaddr_in *)dev_addr->netmask)->sin_addr.s_addr));
                        //printf("\tNetmask: %s\n",iptos(((struct sockaddr_in *)dev_addr->netmask)->sin_addr.s_addr));
                    }
                    if (dev_addr->broadaddr){
                        iface.broadcast = QString(iptos(((struct sockaddr_in *)dev_addr->broadaddr)->sin_addr.s_addr));
                        //printf("\tBroadcast Address: %s\n",iptos(((struct sockaddr_in *)dev_addr->broadaddr)->sin_addr.s_addr));
                    }
                    if (dev_addr->dstaddr){
                        iface.destination = QString(iptos(((struct sockaddr_in *)dev_addr->dstaddr)->sin_addr.s_addr));
                        //printf("\tDestination Address: %s\n",iptos(((struct sockaddr_in *)dev_addr->dstaddr)->sin_addr.s_addr));
                    }
                    ifaceOk = true;
                    break;
//                case AF_PACKET:
//                    //printf("\tAddress Family Name: AF_INET\n");
//                    if (dev_addr->addr){
//                        iface.address = QString(iptos(((struct sockaddr_in *)dev_addr->addr)->sin_addr.s_addr));
//                        //printf("\tAddress: %s\n",iptos(((struct sockaddr_in *)dev_addr->addr)->sin_addr.s_addr));
//                    }
//                    if (dev_addr->netmask){
//                        iface.netmask = QString(iptos(((struct sockaddr_in *)dev_addr->netmask)->sin_addr.s_addr));
//                        //printf("\tNetmask: %s\n",iptos(((struct sockaddr_in *)dev_addr->netmask)->sin_addr.s_addr));
//                    }
//                    if (dev_addr->broadaddr){
//                        iface.broadcast = QString(iptos(((struct sockaddr_in *)dev_addr->broadaddr)->sin_addr.s_addr));
//                        //printf("\tBroadcast Address: %s\n",iptos(((struct sockaddr_in *)dev_addr->broadaddr)->sin_addr.s_addr));
//                    }
//                    if (dev_addr->dstaddr){
//                        iface.destination = QString(iptos(((struct sockaddr_in *)dev_addr->dstaddr)->sin_addr.s_addr));
//                        //printf("\tDestination Address: %s\n",iptos(((struct sockaddr_in *)dev_addr->dstaddr)->sin_addr.s_addr));
//                    }
//                    break;
                case AF_INET6:
                    //printf("\tAddress Family Name: AF_INET6\n");
                    if (dev_addr->addr){
                        //printf("\tAddress: %s\n", ip6tos(dev_addr->addr, ip6str, sizeof(ip6str)));
                        iface.addressIp6 = QString(ip6tos(dev_addr->addr, ip6str, sizeof(ip6str)));
                    }
                    ifaceOk = true;
                    break;
                default:
                    //printf("\tAddress Family Name: Unknown\n");
                    break;
                }
            }
            if(ifaceOk){
                if(dev->name)
                    iface.interfaceName = QString(dev->name);
                if(dev->description)
                    iface.interfaceDescription = QString(dev->description);
                interfaces.push_back(iface);
            }
        }

        pcap_freealldevs(devList);
    }
    return interfaces;
}

void httpsniffer::set_link_offset(int header_type) {

#ifdef DEBUG
    ASSERT(header_type >= 0);
#endif

    switch (header_type) {
    case DLT_EN10MB:
        link_offset = 14;
        break;
#ifdef DLT_IEEE802_11
    case DLT_IEEE802_11:
        link_offset = 32;
        break;
#endif
#ifdef DLT_LINUX_SLL
    case DLT_LINUX_SLL:
        link_offset = 16;
        break;
#endif
#ifdef DLT_LOOP
    case DLT_LOOP:
        link_offset = 4;
        break;
#endif
    case DLT_NULL:
        link_offset = 4;
        break;
    case DLT_RAW:
        link_offset = 0;
        break;
    case DLT_PPP:
        link_offset = 4;
        break;
#ifdef DLT_PPP_SERIAL
    case DLT_PPP_SERIAL:
#endif
    case DLT_PPP_ETHER:
        link_offset = 8;
        break;
    default:
        emit error("Unsupported datalink type: " + QString(pcap_datalink_val_to_name(header_type)));
        break;
    }

    return;
}

/* Find and prepare ethernet device for capturing */
pcap_t *httpsniffer::prepare_capture(char *ethInterface, int promisc, char *filename, char *capfilter) {
    char errbuf[PCAP_ERRBUF_SIZE];
    char *dev = NULL;
    bpf_u_int32 net = 0, mask;
    struct bpf_program filter;

    if (!filename) {
        /* Starting live capture, so find and open network device */
        if (!ethInterface) {
            dev = pcap_lookupdev(errbuf);
            if (dev == NULL){
                emit error("Cannot find a valid capture device: " + QString(errbuf));
                return NULL;
            }
        } else {
            dev = ethInterface;
        }

        if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
            net = 0;

        pcap_hnd = pcap_open_live(dev, BUFSIZ, promisc, 1000, errbuf);

        if (pcap_hnd == NULL){
            emit error("Cannot open live capture on " + QString(dev) + ":" + QString(errbuf));
            return NULL;
        }
    } else {
        /* Reading from a saved capture, so open file */
        pcap_hnd = pcap_open_offline(filename, errbuf);

        if (pcap_hnd == NULL){
            emit error("Cannot open saved capture file: " + QString(errbuf));
            return NULL;
        }
    }

    set_link_offset(pcap_datalink(pcap_hnd));

    /* Compile capture filter and apply to handle */
    if (pcap_compile(pcap_hnd, &filter, capfilter, 0, net) == -1)
        emit error("Cannot compile capture filter " + QString(capfilter) + ":" + QString(pcap_geterr(pcap_hnd)));

    if (pcap_setfilter(pcap_hnd, &filter) == -1)
        emit error("Cannot apply capture filter: " + QString(pcap_geterr(pcap_hnd)));

    pcap_freecode(&filter);

    if (!filename)
        emit error("Starting capture on " + QString(dev) + " interface");

    return pcap_hnd;
}
