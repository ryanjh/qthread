# Qthread = Qt + OpenThread
![Architecture](https://cloud.githubusercontent.com/assets/22163926/26624410/59df27ca-4623-11e7-8ea7-15b68b3f2f71.JPG)

Environment (Docker qthread:16.04_dev): https://hub.docker.com/r/ryanjh31011/qthread/
1. docker run -v ~/Desktop/qthread/ryan:/home/ryan -d -P ryanjh31011/qthread:16.04_dev /usr/sbin/sshd -D
2. ssh -p 32779 root@192.168.99.100 #The Port and IP address should be set accordantly
3. adduser ryan #Add your user account
4. su - ryan    #Switch to your account
5. git clone https://github.com/ryanjh/qthread.git

Build (GitHub qt_console): https://github.com/ryanjh/qthread/tree/master/qt_console
1. cd qthread/
2. /opt/Qt/5.8/gcc_64/bin/qmake -o build/Makefile qt_console/qt_console.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
3. make -C build/

Test:
1. ./build/qt_console
2. /usr/local/src/openthread/output/x86_64-unknown-linux-gnu/bin/ot-cli-ftd 2 (>panid 0x1234, >ifconfig up, >thread start)
