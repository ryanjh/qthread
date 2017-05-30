#include <QCoreApplication>
#include <iostream>

#include <openthread/platform/platform.h>

#include <qt_console.h>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout<< "Hello Ryan\n";

    PlatformInit(argc, argv);

    return a.exec();
}
