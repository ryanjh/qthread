#include <QCoreApplication>
#include <iostream>
#include <qt_console.h>

// OpenThread API
#include <openthread/openthread.h>
#include <openthread/platform/platform.h>

using namespace std;

void otTaskletsSignalPending(otInstance *aInstance)
{
    (void)aInstance;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout<< "Hello Qthread!\n";

    PlatformInit(argc, argv);

    return a.exec();
}
