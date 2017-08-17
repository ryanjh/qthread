#include <QCoreApplication>
#include <QString>
#include <iostream>
#include <assert.h>

#include <qthread.hpp>
#include <qt_console.h>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout<< "Hello Qt_console!\n";

    for (int i = 1; i < argc; i++)
    {
        switch(static_cast<Argv>(i))
        {
            case Argv::MAX_NODES:
            WELLKNOWN_NODE_ID = QString(argv[i]).toUInt();
            break;
        }
    }

    Qthread qthread;
    qthread.listIpAddr();

    Qthread qthread2(2);
    qthread2.listIpAddr();

    return a.exec();
}
