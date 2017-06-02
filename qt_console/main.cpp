#include <QCoreApplication>
#include <iostream>

#ifdef OPENTHREAD_CONFIG_FILE
#include OPENTHREAD_CONFIG_FILE
#else
//#include <openthread-config.h>
#endif

#include <assert.h>

#include <openthread/cli.h>
#include <openthread/diag.h>
#include <openthread/openthread.h>
#include <openthread/platform/platform.h>

#ifdef OPENTHREAD_MULTIPLE_INSTANCE
void *otPlatCAlloc(size_t aNum, size_t aSize)
{
    return calloc(aNum, aSize);
}

void otPlatFree(void *aPtr)
{
    free(aPtr);
}
#endif

void otTaskletsSignalPending(otInstance *aInstance)
{
    (void)aInstance;
}

#include <qt_console.h>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout<< "Hello Ryan\n";

    PlatformInit(argc, argv);

    return a.exec();
}
