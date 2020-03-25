#ifndef INTERFACEINFO_H
#define INTERFACEINFO_H
#include <QString>
struct InterfaceInfo{
    QString interfaceName;
    QString interfaceDescription;
    QString address;
    QString addressIp6;
    QString netmask;
    QString broadcast;
    QString destination ;
};

#endif // INTERFACEINFO_H
