#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <vector>

#include <qthread.hpp>
#include "qt_console.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInfo("Hello Qt_console!");

    quint32 nodes = 1;

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

    std::vector<Qthread> myNodes(nodes);
    for (auto &node : myNodes)
    {
        qInfo() << node.getRole();
        qInfo("Device RLOC %#x", node.getRloc());

        for (auto addr : node.getIpAddress())
        {
            qInfo() << addr;
        }
    }

    // ping children with their GUA address
    if (myNodes.size() > 1)
    {
        auto leader = myNodes.begin();
        for (auto child = leader + 1; child != myNodes.end(); child++)
        {
            QString pingAddress = child->getGlobalAddress();
            if (pingAddress.length())
            {
                leader->ping(pingAddress);
            }
        }
    }

    return a.exec();
}
