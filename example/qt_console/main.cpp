#include <QCoreApplication>
#include <QThread>
#include <iostream>
#include <assert.h>
#include <qt_console.h>

// OpenThread API
#include <openthread/openthread.h>
#include <openthread/platform/platform.h>

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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout<< "Hello Qt_console!\n";

    if (argc < 2)
    {
        char *argv_default[2] = {argv[0], (char*) "1"};
        PlatformInit(2, argv_default);
    }
    else
    {
        PlatformInit(argc, argv);
    }

    otInstance *sInstance = otInstanceInit();
    assert(sInstance);

    SystemThread system_thread(sInstance);

    printf("otLinkSetPanId (%d)\n", otLinkSetPanId(sInstance, static_cast<otPanId>(0x1234)));
    printf("panid = 0x%x\n", otLinkGetPanId(sInstance));

    if (!otIp6IsEnabled(sInstance))
    {
        printf("otIp6SetEnabled (%d)\n", otIp6SetEnabled(sInstance, true));
    }
    printf("ifconfig up\n");

    printf("otThreadSetEnabled (%d)\n", otThreadSetEnabled(sInstance, true));
    printf("thread start\n");

    system_thread.start();

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

    while (true)
    {
        //NOOP
    }

    return a.exec();
}
