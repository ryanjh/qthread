#include <QThread>
#include <QtEndian>
#include <iostream>
#include <assert.h>

// OpenThread API
#include <openthread/openthread.h>
#include <openthread/platform/platform.h>
#include <openthread/diag.h>

#include <qthread.hpp>

using namespace std;

class SystemThread : public QThread
{
public:
    SystemThread(const Qthread &qInstance) : qthread(qInstance) {}

private:
    const Qthread &qthread;

    void run()
    {
        while (qthread.instance)
        {
            otInstance *sysInstance = static_cast< otInstance* > (qthread.instance);
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
}

Qthread::Qthread(uint32_t node_id)
{
    char *argv_default[2] = {(char*) "qthread.cpp",
                             (char*) QString("%1").arg(node_id).toStdString().c_str()};
    PlatformInit(2, argv_default);

    printf("nodeid = %#x\n", NODE_ID);

    // Call to query the buffer size
    size_t instanceBufferLength = 0;
    (void)otInstanceInit(NULL, &instanceBufferLength);

    // Call to allocate the buffer
    instanceBuffer = new uint8_t[instanceBufferLength];
    assert(instanceBuffer);

    // Initialize OpenThread with the buffer
    otInstance *sInstance = otInstanceInit(instanceBuffer, &instanceBufferLength);
    assert(sInstance);

    // Create system thread
    system_thread = new SystemThread(*this);
    assert(system_thread);

    // Start system thread for task scheduling
    instance = static_cast< void* > (sInstance);
    system_thread->start();

    printf("otLinkSetPanId (%d)\n", otLinkSetPanId(sInstance, static_cast<otPanId>(0x1234)));
    printf("panid = %#x\n", otLinkGetPanId(sInstance));

    if (!otIp6IsEnabled(sInstance))
    {
        printf("otIp6SetEnabled (%d)\n", otIp6SetEnabled(sInstance, true));
    }
    printf("ifconfig up\n");

    printf("otThreadSetEnabled (%d)\n", otThreadSetEnabled(sInstance, true));

    bool poll_devRole = true;
    while (poll_devRole)
    {
        printf("waiting thread network ...\n");
        QThread::sleep(1);

        switch (otThreadGetDeviceRole(sInstance))
        {
        case OT_DEVICE_ROLE_DISABLED:
            printf("state disabled\n");
            break;

        case OT_DEVICE_ROLE_DETACHED:
            printf("state detached\n");
            break;

        case OT_DEVICE_ROLE_CHILD:
            printf("state child\n");
            break;

        case OT_DEVICE_ROLE_ROUTER:
            printf("state router, rloc16 = %#x\n", otThreadGetRloc16(sInstance));
            poll_devRole = false;
            break;

        case OT_DEVICE_ROLE_LEADER:
            printf("state leader, rloc16 = %#x\n", otThreadGetRloc16(sInstance));
            poll_devRole = false;
            break;

        default:
            printf("invalid state\n");
            poll_devRole = false;
            break;
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

    delete [] instanceBuffer;
}

void Qthread::listIpAddr(void)
{
    otInstance *sInstance = static_cast< otInstance* > (instance);
    if (sInstance)
    {
        for (const otNetifAddress *addr = otIp6GetUnicastAddresses(sInstance); addr; addr = addr->mNext)
        {
            printf("%x:%x:%x:%x:%x:%x:%x:%x\r\n",
                qToBigEndian(addr->mAddress.mFields.m16[0]),
                qToBigEndian(addr->mAddress.mFields.m16[1]),
                qToBigEndian(addr->mAddress.mFields.m16[2]),
                qToBigEndian(addr->mAddress.mFields.m16[3]),
                qToBigEndian(addr->mAddress.mFields.m16[4]),
                qToBigEndian(addr->mAddress.mFields.m16[5]),
                qToBigEndian(addr->mAddress.mFields.m16[6]),
                qToBigEndian(addr->mAddress.mFields.m16[7]));
        }
    }
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
        char *output = NULL;

        memcpy(string, tests[i].command, strlen(tests[i].command) + 1);

        output = otDiagProcessCmdLine(string);
        cout << output << endl;
    }
}
