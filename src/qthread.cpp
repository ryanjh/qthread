#include <QThread>
#include <QtEndian>
#include <QString>
#include <iostream>
#include <assert.h>

// OpenThread API
#include <openthread/openthread.h>
#include <openthread/platform/platform.h>
#include <openthread/diag.h>

#include <qthread.hpp>

/**
 * Unique node ID.
 *
 */
extern uint32_t NODE_ID;

using namespace std;

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
                    printf("switch disabled\n");
                    break;

                case OT_DEVICE_ROLE_DETACHED:
                    printf("switch detached\n");
                    break;

                case OT_DEVICE_ROLE_CHILD:
                    printf("switch child, rloc16 = %#x\n", otThreadGetRloc16(sysInstance));
                    break;

                case OT_DEVICE_ROLE_ROUTER:
                    printf("switch router, rloc16 = %#x\n", otThreadGetRloc16(sysInstance));
                    break;

                case OT_DEVICE_ROLE_LEADER:
                    printf("switch leader, rloc16 = %#x\n", otThreadGetRloc16(sysInstance));
                    break;

                default:
                    printf("switch unknown (%d)\n", newRole);
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
}

Qthread::Qthread(uint32_t node_id)
{
    char *argv_default[2] = {(char*) "qthread.cpp",
                             (char*) QString("%1").arg(node_id).toStdString().c_str()};

    PlatformInit(2, argv_default);

    // Call to query the buffer size
    size_t pfBufferLength = 0;
    (void)PlatformAlloc(NULL, &pfBufferLength);

    size_t otBufferLength = 0;
    (void)otInstanceInit(NULL, &otBufferLength);

    // Call to allocate the buffer
    instanceBuffer = new uint8_t[pfBufferLength + otBufferLength] {};
    assert(instanceBuffer);

    // Initialize platform with the buffer
    otInstance *sInstance = PlatformAlloc(instanceBuffer, &pfBufferLength);
    assert(sInstance);

    // Initialize OpenThread with the buffer
    sInstance = otInstanceInit(sInstance, &otBufferLength);
    printf("sInstance = %p\n", sInstance);
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

    printf("waiting thread network ...\n");
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
        QThread::sleep(1);
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
