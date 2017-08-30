#include <QtEndian>
#include <QString>
#include <QDebug>

// OpenThread API
#include <openthread/openthread.h>
#include <openthread/platform/alarm.h>
#include <openthread/icmp6.h>

// Qthread API
#include <qthread.hpp>

#include "icmpv6.hpp"

otIcmp6Handler mIcmpHandler;

void s_HandleIcmpReceive(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo, const otIcmp6Header *aIcmpHeader)
{
    QString outStr;
    QTextStream out(&outStr);
    uint32_t timestamp = 0;
    (void) aContext;

    switch(aIcmpHeader->mType)
    {
        case OT_ICMP6_TYPE_DST_UNREACH:
            out << "[PING_UNREACH] ";
            break;
        case OT_ICMP6_TYPE_ECHO_REQUEST:
            out << "[PING_REQ] ";
            break;
        case OT_ICMP6_TYPE_ECHO_REPLY:
            out << "[PING_REP] ";
            break;
    }

    out << QString("%1 bytes from %2:%3:%4:%5:%6:%7:%8:%9 ")
           .arg(otMessageGetLength(aMessage) - otMessageGetOffset(aMessage))
           .arg(qToBigEndian(aMessageInfo->mPeerAddr.mFields.m16[0]), 0, 16)
           .arg(qToBigEndian(aMessageInfo->mPeerAddr.mFields.m16[1]), 0, 16)
           .arg(qToBigEndian(aMessageInfo->mPeerAddr.mFields.m16[2]), 0, 16)
           .arg(qToBigEndian(aMessageInfo->mPeerAddr.mFields.m16[3]), 0, 16)
           .arg(qToBigEndian(aMessageInfo->mPeerAddr.mFields.m16[4]), 0, 16)
           .arg(qToBigEndian(aMessageInfo->mPeerAddr.mFields.m16[5]), 0, 16)
           .arg(qToBigEndian(aMessageInfo->mPeerAddr.mFields.m16[6]), 0, 16)
           .arg(qToBigEndian(aMessageInfo->mPeerAddr.mFields.m16[7]), 0, 16);

    out << QString(": icmp_seq=%1 hlim=%2")
           .arg(qToBigEndian(aIcmpHeader->mData.m16[1]))
           .arg(aMessageInfo->mHopLimit);

    if (otMessageRead(aMessage, otMessageGetOffset(aMessage), &timestamp, sizeof(uint32_t) >=
        static_cast<int>(sizeof(uint32_t))))
    {
        out << QString(" time=%1ms").arg(otPlatAlarmGetNow() - qToBigEndian(timestamp));
    }
    qDebug() << outStr;
}

void Qthread::ping(QString ipAddress, quint16 length)
{
    otInstance *sInstance = static_cast< otInstance* > (instance);
    otError error = OT_ERROR_NONE;
    uint32_t timestamp = qToBigEndian(otPlatAlarmGetNow());
    otMessage *message = otIp6NewMessage(sInstance, true);

    if (message != nullptr)
    {
        if ((error = otMessageAppend(message, &timestamp, sizeof(timestamp))) == OT_ERROR_NONE)
        {
            if ((error = otMessageSetLength(message, length)) == OT_ERROR_NONE)
            {
                otMessageInfo messageInfo = {};
                error = otIp6AddressFromString(ipAddress.toStdString().c_str(), &messageInfo.mPeerAddr);
                if (error == OT_ERROR_NONE)
                {
                    messageInfo.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
                    error = otIcmp6SendEchoRequest(sInstance, message, &messageInfo, 1);
                }
            }
        }
    }

    if (error != OT_ERROR_NONE && message != nullptr)
    {
        otMessageFree(message);
    }
}
