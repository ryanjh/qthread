#include <QThread>
#include <QtEndian>
#include <QString>
#include <QDebug>
#include <assert.h>

// OpenThread API
#include <openthread/openthread.h>
#include <openthread/platform/platform.h>
#include <openthread/diag.h>
#include <openthread/icmp6.h>
#include <openthread/ip6.h>

#include <qthread.hpp>

class SystemThread : public QThread
{
public:
    SystemThread(const Qthread &qInstance) : qthread(qInstance), deviceRole(OT_DEVICE_ROLE_DISABLED) {}

private:
    const Qthread &qthread;
    otDeviceRole deviceRole;

    void run()
    {
        while (qthread.instance)
        {
            otInstance *sysInstance = static_cast< otInstance* > (qthread.instance);

            otDeviceRole newRole = otThreadGetDeviceRole(sysInstance);
            if (deviceRole != newRole)
            {
                switch (newRole)
                {
                case OT_DEVICE_ROLE_DISABLED:
                    qDebug("switch disabled\n");
                    break;

                case OT_DEVICE_ROLE_DETACHED:
                    qDebug("switch detached\n");
                    break;

                case OT_DEVICE_ROLE_CHILD:
                    qDebug("switch child, rloc16 = %#x\n", otThreadGetRloc16(sysInstance));
                    break;

                case OT_DEVICE_ROLE_ROUTER:
                    qDebug("switch router, rloc16 = %#x\n", otThreadGetRloc16(sysInstance));
                    break;

                case OT_DEVICE_ROLE_LEADER:
                    qDebug("switch leader, rloc16 = %#x\n", otThreadGetRloc16(sysInstance));
                    break;

                default:
                    qDebug("switch unknown (%d)\n", newRole);
                }
                deviceRole = newRole;
            }

            otTaskletsProcess(sysInstance);
            PlatformProcessDrivers(sysInstance);
        }
    }
};

void otTaskletsSignalPending(otInstance *aInstance)
{
    (void)aInstance;
}

extern "C" void otPlatUartReceived(const uint8_t *, uint16_t)
{
    //-lopenthread-cli-ftd
}

extern "C" void otPlatUartSendDone(void)
{
    //-lopenthread-cli-ftd
}

Qthread::Qthread() : Qthread(NODE_ID)
{
    NODE_ID++;
}

Qthread::Qthread(quint32 node_id)
{
    char *argv_default[2] = {(char*) __BASE_FILE__,
                             (char*) QString("%1").arg(node_id).toStdString().c_str()};

    PlatformInit(2, argv_default);

    // Call to query the buffer size
    size_t pfBufferLength = 0;
    (void)PlatformAlloc(nullptr, &pfBufferLength);

    size_t otBufferLength = 0;
    (void)otInstanceInit(nullptr, &otBufferLength);

    // Call to allocate the buffer
    instanceBuffer = new quint8[pfBufferLength + otBufferLength] {};
    assert(instanceBuffer);

    // Initialize platform with the buffer
    otInstance *sInstance = PlatformAlloc(instanceBuffer, &pfBufferLength);
    assert(sInstance);

    // Initialize OpenThread with the buffer
    sInstance = otInstanceInit(sInstance, &otBufferLength);
    qDebug("sInstance = %p\n", sInstance);
    assert(sInstance);

    // Initialize default log level
    otSetDynamicLogLevel(sInstance, OT_LOG_LEVEL_NONE);

    // Create system thread
    system_thread = new SystemThread(*this);
    assert(system_thread);

    qDebug("otLinkSetPanId (%d)\n", otLinkSetPanId(sInstance, static_cast<otPanId>(0x1234)));
    qDebug("panid = %#x\n", otLinkGetPanId(sInstance));

    if (!otIp6IsEnabled(sInstance))
    {
        qDebug("otIp6SetEnabled (%d)\n", otIp6SetEnabled(sInstance, true));
    }
    qDebug("ifconfig up\n");

    qDebug("otThreadSetEnabled (%d)\n", otThreadSetEnabled(sInstance, true));

    qDebug("waiting thread network ...\n");

    // Start system thread for task scheduling
    instance = static_cast< void* > (sInstance);
    system_thread->start();

    bool poll_devRole = true;
    while (poll_devRole)
    {
        switch (otThreadGetDeviceRole(sInstance))
        {
        case OT_DEVICE_ROLE_DISABLED:
        case OT_DEVICE_ROLE_DETACHED:
            break;

        case OT_DEVICE_ROLE_CHILD:
        case OT_DEVICE_ROLE_ROUTER:
        case OT_DEVICE_ROLE_LEADER:
            poll_devRole = false;
            break;

        default:
            throw("Error: thread network returns fail\n");
        }
    }

    // initialize diagnostics module
    otDiagInit(sInstance);
}

Qthread::~Qthread()
{
    otInstance *sInstance = static_cast< otInstance* > (instance);

    if (system_thread)
    {
        //stop running loop of system thread
        instance = nullptr;

        if(!system_thread->wait(5000))
        {
            system_thread->terminate();
            system_thread->wait();
        }
    }

    if (sInstance)
    {
        otInstanceFinalize(sInstance);
    }

    delete system_thread;
    delete [] instanceBuffer;
}

bool Qthread::operator==(const Qthread& qthread)
{
    if (instance == qthread.instance)
    {
        return true;
    }
    return false;
}

void Qthread::sanityTest(void)
{
    static const struct
    {
        const char *command;
        const char *output;
    } tests[] =
    {
        {
            "diag\n",
            "diagnostics mode is disabled\r\n",
        },
        {
            "diag send 10 100\n",
            "failed\r\nstatus 0xd\r\n",
        },
        {
            "diag start\n",
            "start diagnostics mode\r\nstatus 0x00\r\n",
        },
        {
            "diag\n",
            "diagnostics mode is enabled\r\n",
        },
        {
            "diag channel 10\n",
            "failed\r\nstatus 0x7\r\n",
        },
        {
            "diag channel 11\n",
            "set channel to 11\r\nstatus 0x00\r\n",
        },
        {
            "diag channel\n",
            "channel: 11\r\n",
        },
        {
            "diag power -10\n",
            "set tx power to -10 dBm\r\nstatus 0x00\r\n",
        },
        {
            "diag power\n",
            "tx power: -10 dBm\r\n",
        },
        {
            "diag stats\n",
            "received packets: 0\r\nsent packets: 0\r\nfirst received packet: rssi=0, lqi=0\r\n",
        },
        {
            "diag send 20 100\n",
            "sending 0x14 packet(s), length 0x64\r\nstatus 0x00\r\n",
        },
        {
            "diag repeat 100 100\n",
            "sending packets of length 0x64 at the delay of 0x64 ms\r\nstatus 0x00\r\n"
        },
        {
            "diag sleep\n",
            "sleeping now...\r\n",
        },
        {
            "diag stop\n",
            "received packets: 0\r\nsent packets: 0\r\nfirst received packet: rssi=0, lqi=0\r\n\nstop diagnostics mode\r\nstatus 0x00\r\n",
        },
        {
            "diag\n",
            "diagnostics mode is disabled\r\n",
        },
    };

    for (unsigned int i = 0; i < sizeof(tests) / sizeof(tests[0]);  i++)
    {
        char string[50];
        char *output = nullptr;

        memcpy(string, tests[i].command, strlen(tests[i].command) + 1);

        output = otDiagProcessCmdLine(string);
        qDebug() << output;
    }
}

QList<QString> Qthread::getIpAddress(void)
{
    QList<QString> addrList;

    otInstance *sInstance = static_cast< otInstance* > (instance);
    if (sInstance)
    {
        for (const otNetifAddress *addr = otIp6GetUnicastAddresses(sInstance); addr; addr = addr->mNext)
        {
            QString str = QString("%1:%2:%3:%4:%5:%6:%7:%8")
                    .arg(qToBigEndian(addr->mAddress.mFields.m16[0]), 0, 16)
                    .arg(qToBigEndian(addr->mAddress.mFields.m16[1]), 0, 16)
                    .arg(qToBigEndian(addr->mAddress.mFields.m16[2]), 0, 16)
                    .arg(qToBigEndian(addr->mAddress.mFields.m16[3]), 0, 16)
                    .arg(qToBigEndian(addr->mAddress.mFields.m16[4]), 0, 16)
                    .arg(qToBigEndian(addr->mAddress.mFields.m16[5]), 0, 16)
                    .arg(qToBigEndian(addr->mAddress.mFields.m16[6]), 0, 16)
                    .arg(qToBigEndian(addr->mAddress.mFields.m16[7]), 0, 16);

            addrList.append(str);
        }
    }
    return addrList;
}

QString Qthread::getRole(void)
{
    otInstance *sInstance = static_cast< otInstance* > (instance);
    if (sInstance)
    {
        switch (otThreadGetDeviceRole(sInstance))
        {
        case OT_DEVICE_ROLE_DISABLED:
            return QString("OT_DEVICE_ROLE_DISABLED");

        case OT_DEVICE_ROLE_DETACHED:
            return QString("OT_DEVICE_ROLE_DETACHED");

        case OT_DEVICE_ROLE_CHILD:
            return QString("OT_DEVICE_ROLE_CHILD");

        case OT_DEVICE_ROLE_ROUTER:
            return QString("OT_DEVICE_ROLE_ROUTER");

        case OT_DEVICE_ROLE_LEADER:
            return QString("OT_DEVICE_ROLE_LEADER");

        default:
            return QString("Unknown");
        }
    }
    return QString("NULL instance");
}

quint16 Qthread::getRloc(void)
{
    otInstance *sInstance = static_cast< otInstance* > (instance);
    if (sInstance)
    {
        return otThreadGetRloc16(sInstance);
    }
    return 0xffff;
}
