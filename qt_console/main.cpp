#include <QCoreApplication>
#include <iostream>
#include <qt_console.h>

#include <assert.h>

// OpenThread API
#include <openthread/openthread.h>
#include <openthread/platform/platform.h>

using namespace std;

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
    cout<< "Hello Qthread!\n";

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

    return a.exec();
}
