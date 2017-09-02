#include <QString>
#include <QDebug>
#include <QHostAddress>
#include <QNetworkInterface>

// Qthread API
#include <qthread.hpp>

#include "network.hpp"

void Qthread::setupNetwork(QString interface)
{
    for (QNetworkInterface netInterface : QNetworkInterface::allInterfaces()) {
        if (!netInterface.isValid() || netInterface.name() != interface)
            continue;

        for(QNetworkAddressEntry entry : netInterface.addressEntries()) {
            bool isIPv4;
            quint32 ipAddr = entry.ip().toIPv4Address(&isIPv4);
            quint32 netMask = entry.netmask().toIPv4Address();

            if (isIPv4)
            {
                // Set broadcast address of this interface
                qputenv("OT_ADDRESS", QHostAddress(ipAddr | ~netMask).toString().toLocal8Bit());
            }
        }
    }
    qDebug() << "ENV OT_ADDRESS:" << qgetenv("OT_ADDRESS");
}
