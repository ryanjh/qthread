# Qthread = Qt + OpenThread
![Architecture](https://cloud.githubusercontent.com/assets/22163926/26624410/59df27ca-4623-11e7-8ea7-15b68b3f2f71.JPG)

Environment (Docker qthread): https://hub.docker.com/r/ryanjh31011/qthread/
1. docker run -v ~/Desktop/qthread/ryan:/home/ryan -d -P ryanjh31011/qthread:Ubuntu16.04_latest /usr/sbin/sshd -D  [Ubuntu 16.04]
1. docker run -v ~/Desktop/qthread/ryan:/home/ryan -d -P ryanjh31011/qthread:RaspberryPi2_latest /usr/sbin/sshd -D [Raspberry Pi2]
2. ssh -p 32779 root@192.168.99.100 #The Port and IP address should be set accordantly
3. adduser ryan #Add your user account
4. adduser ryan sudo
5. su - ryan    #Switch to your account
6. git clone https://github.com/ryanjh/qthread.git
7. cd qthread/

Build (GitHub libqthread.a) https://github.com/ryanjh/qthread/tree/qthread_library/src
1. [Ubuntu 16.04] Build debugging library (build/qthread/libqthread.a)
   /opt/Qt/5.8/gcc_64/bin/qmake -o build/qthread/Makefile qthread.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug

1. [Raspberry Pi2] Build debugging library (build/qthread/libqthread.a)
   /opt/qtrpi/bin/qmake-qtrpi -o build/qthread/Makefile qthread.pro -spec devices/linux-rasp-pi2-g++ CONFIG+=debug CONFIG+=qml_debug

2. make -C build/qthread/

Build (GitHub qt_console): https://github.com/ryanjh/qthread/tree/master/qt_console
1. [Ubuntu 16.04] Link to debugging library (build/qthread/libqthread.a)
   /opt/Qt/5.8/gcc_64/bin/qmake -o build/qt_console/Makefile example/qt_console/qt_console.pro -spec linux-g++ CONFIG+=debug_lib CONFIG+=debug CONFIG+=qml_debug

1. [Raspberry Pi2] Link to debugging library (build/qthread/libqthread.a)
   /opt/qtrpi/bin/qmake-qtrpi -o build/qt_console/Makefile example/qt_console/qt_console.pro -spec devices/linux-rasp-pi2-g++ CONFIG+=debug_lib CONFIG+=debug CONFIG+=qml_debug

2. make -C build/qt_console/

Release qthread library:

https://github.com/ryanjh/qthread/blob/qthread_rspi/lib/README.md

Test:
1. (Docker Container 1) ./build/qt_console/qt_console 2 (Create two devices, one becomes leader and the other one becomes child->router)
2. (Docker Container 2) ./build/qt_console/qt_console 3 5 (Create threes devices connecting to the leader of container 1,  All child->router)

(Appendix) Install qthread library [deprecated]
1. sudo make -C build/qthread/ install  #Install /usr/local/lib/qthread/libqthread.a

(Appendix) Link to installed library (/usr/local/lib/qthread/libqthread.a) [deprecated]
1. /opt/Qt/5.8/gcc_64/bin/qmake -o build/qt_console/Makefile example/qt_console/qt_console.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
2. make -C build/qt_console/

(Appendix) Qt5 Run-time Environment [Raspberry Pi2]:
1. download lib/qt5pi_rpi2.tar.gz cross-compiled share libraries to target board
2. tar zxvf lib/qt5pi_rpi2.tar.gz
3. export LD_LIBRARY_PATH=qt5pi/lib/
