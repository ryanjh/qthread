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

    return a.exec();
}
