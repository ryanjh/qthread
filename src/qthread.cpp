#include <QThread>
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
    SystemThread(otInstance *instance) : sysInstance(instance) {}

private:
    otInstance *sysInstance = NULL;

    void run()
    {
        while (sysInstance)
        {
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

uint32_t Qthread::internal_nodeId = NODE_ID;

Qthread::Qthread()
{
    char *argv_default[2] = {(char*) "qthread.cpp", (char*) "1"};
    PlatformInit(2, argv_default);

    printf("nodeid = 0x%x\n", Qthread::internal_nodeId++);

    otInstance *sInstance = otInstanceInit();
    assert(sInstance);

    system_thread = (QThread*) new SystemThread(sInstance);

    printf("otLinkSetPanId (%d)\n", otLinkSetPanId(sInstance, static_cast<otPanId>(0x1234)));
    printf("panid = 0x%x\n", otLinkGetPanId(sInstance));

    if (!otIp6IsEnabled(sInstance))
    {
        printf("otIp6SetEnabled (%d)\n", otIp6SetEnabled(sInstance, true));
    }
    printf("ifconfig up\n");

    printf("otThreadSetEnabled (%d)\n", otThreadSetEnabled(sInstance, true));
    printf("thread start\n");

    system_thread->start();

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
            printf("state router\n");
            poll_devRole = false;
            break;

        case OT_DEVICE_ROLE_LEADER:
            printf("state leader\n");
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
            printf("[%s]\n", output);
        }


}

Qthread::~Qthread()
{
    //TODO stop system_thread
}
