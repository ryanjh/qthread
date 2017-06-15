# Qthread = Qt + OpenThread
![Architecture](https://cloud.githubusercontent.com/assets/22163926/26624410/59df27ca-4623-11e7-8ea7-15b68b3f2f71.JPG)

Build (qt_console):
1. cd qthread/
2. /opt/Qt/5.8/gcc_64/bin/qmake -o build/Makefile qt_console/qt_console.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
3. make -C build/

Test:
1. ./build/qt_console
2. /usr/local/src/openthread/output/x86_64-unknown-linux-gnu/bin/ot-cli-ftd 2 (>panid 0x1234, >ifconfig up, >thread start)

Log:
Hello Qt_console!
otLinkSetPanId (0)
panid = 0x1234
otIp6SetEnabled (0)
ifconfig up
otThreadSetEnabled (0)
thread start
waiting thread network ...
state detached
waiting thread network ...
state detached
waiting thread network ...
state leader
