# Qthread = Qt + OpenThread
![Architecture](https://cloud.githubusercontent.com/assets/22163926/26624410/59df27ca-4623-11e7-8ea7-15b68b3f2f71.JPG)

Environment (Docker qthread:Ubuntu16.04_40ff2d8): https://hub.docker.com/r/ryanjh31011/qthread/
1. docker run -v ~/Desktop/qthread/ryan:/home/ryan -d -P ryanjh31011/qthread:16.04_dev /usr/sbin/sshd -D
2. ssh -p 32779 root@192.168.99.100 #The Port and IP address should be set accordantly
3. adduser ryan #Add your user account
4. adduser qthread sudo
5. su - ryan    #Switch to your account
6. git clone https://github.com/ryanjh/qthread.git

Build (GitHub libqthread.a) https://github.com/ryanjh/qthread/tree/qthread_library/src
1. /opt/Qt/5.8/gcc_64/bin/qmake -o build/qthread/Makefile qthread.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
2. make -C build/qthread/               #Build   build/qthread/libqthread.a
3. make -C build/qthread/ release       #Release lib/libqthread_Linux_x86_64.a
3. sudo make -C build/qthread/ install  #Install /usr/local/lib/qthread/libqthread.a

Build (GitHub qt_console): https://github.com/ryanjh/qthread/tree/master/qt_console
1. cd qthread/
2. /opt/Qt/5.8/gcc_64/bin/qmake -o build/qt_console/Makefile example/qt_console/qt_console.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
3. OR /opt/Qt/5.8/gcc_64/bin/qmake -o build/qt_console/Makefile example/qt_console/qt_console.pro -spec linux-g++ CONFIG+=debug_lib CONFIG+=debug CONFIG+=qml_debu
4. OR /opt/Qt/5.8/gcc_64/bin/qmake -o build/qt_console/Makefile example/qt_console/qt_console.pro -spec linux-g++ CONFIG+=release_Linux_x86_64 CONFIG+=qml_debu
5. make -C build/qt_console/

Test:
1. build/qt_console/qt_console
2. /usr/local/src/openthread/output/x86_64-unknown-linux-gnu/bin/ot-cli-ftd 2 (>panid 0x1234, >ifconfig up, >thread start)
