#include <QCoreApplication>
#include <QDebug>
#include "pcap.h"
#include "pcap-stdinc.h"

#define HAVE_REMOTE
#include "remote-ext.h"



int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
    pcap_if_t *alldevs;
    pcap_if_t *d;
    int inum;
    int i = 0;
    pcap_t *adhandle;
    char errbuf[PCAP_ERRBUF_SIZE];
    u_int netmask;
//    char packet_filter[] = "eth.type==0xffaa";
    char packet_filter[] = "ether proto 0xFFAA";
    struct bpf_program fcode;

    if(pcap_findalldevs_ex(PCAP_SRC_IF_STRING, nullptr, &alldevs, errbuf) == -1){
        qDebug() << "Error in pcap_findalldevs: " << errbuf;
        return 0;
    }
    for (d=alldevs; d; d=d->next) {
        qDebug() << ++i << ", " << d->name;
        if(d->description){
            qDebug() << d->description;
        }else{
            qDebug() << "No description available";
        }
    }
    if(i == 0){
        qDebug() << "No interfaces found!";
        return 0;
    }
    qDebug() << "Enter the interface number: ";
    scanf("%d", &inum);
    if(inum < 1 || inum > i){
        qDebug() << "Interface number out of range.";
        pcap_freealldevs(alldevs);
        return 0;
    }
    for(d=alldevs, i=0; i<inum-1; d=d->next, i++);
    if((adhandle = pcap_open(d->name,
                             65536,
                             PCAP_OPENFLAG_PROMISCUOUS,
                             1000,
                             nullptr,
                             errbuf)) == nullptr){
        qDebug() << "Unable to open the adapter.";
        pcap_freealldevs(alldevs);
        return 0;
    }
    if(pcap_datalink(adhandle) != DLT_EN10MB){
        qDebug() << "This program works only on Ethernet networks.";
        pcap_freealldevs(alldevs);
        return 0;
    }
    if(d->addresses != nullptr){
        netmask = ((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
    }else{
        netmask = 0xffffff;
    }
    if(pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0){
        qDebug() << "Unable to compile the packet filter.";
        pcap_freealldevs(alldevs);
        return 0;
    }
    if(pcap_setfilter(adhandle, &fcode) < 0){
        qDebug() << "Error setting the filter.";
        return 0;
    }

    pcap_freealldevs(alldevs);







//    return a.exec();
    return 0;
}
