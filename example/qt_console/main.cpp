#include <QCoreApplication>
#include <iostream>
#include <assert.h>

#include <qthread.hpp>
#include <qt_console.h>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout<< "Hello Qt_console!\n";

    Qthread qthread;
    qthread.listIpAddr();
#if 1
    Qthread qthread2(2);
    qthread2.listIpAddr();

    if (qthread == qthread2)
    {
        cout << "[yes]" << endl;
    }
    else
    {
        cout << "[no]" << endl;
    }
#endif
    return a.exec();
}
