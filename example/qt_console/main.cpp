#include <QCoreApplication>
#include <QString>
#include <vector>
#include <iostream>
#include <assert.h>

#include <qthread.hpp>
#include <qt_console.h>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cout<< "Hello Qt_console!\n";

    uint32_t nodes = 1;

    for (int i = 1; i < argc; i++)
    {
        switch(static_cast<Argv>(i))
        {
        case Argv::MAX_NODES:
            WELLKNOWN_NODE_ID = QString(argv[i]).toUInt();
            break;
        case Argv::START_NODEID:
            NODE_ID = QString(argv[i]).toUInt();
            break;
        case Argv::CREATE_NODES:
            nodes = QString(argv[i]).toUInt();
            break;
        }
    }

    vector<Qthread> myNodes(nodes);
    for (auto &node : myNodes)
    {
        node.listIpAddr();
    }

    return a.exec();
}
