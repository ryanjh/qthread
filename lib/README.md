[Ubuntu 16.04]

Build release library (lib/libqthread_Linux_x86_64.a)
1. /opt/Qt/5.8/gcc_64/bin/qmake -o build/qthread/Makefile qthread.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
2. make -C build/qthread/ release

Link to released library (lib/libqthread_Linux_x86_64.a)
1. /opt/Qt/5.8/gcc_64/bin/qmake -o build/qt_console/Makefile example/qt_console/qt_console.pro -spec linux-g++ CONFIG+=release_Linux_x86_64 CONFIG+=qml_debug
2. make -C build/qt_console/

---

[Raspberry Pi 2]

Build release library (lib/libqthread_Raspberry_Pi2.a)
1. /opt/qtrpi/bin/qmake-qtrpi -o build/qthread/Makefile qthread.pro -spec devices/linux-rasp-pi2-g++ CONFIG+=debug CONFIG+=qml_debug
2. make -C build/qthread/ release_rpi2

Link to released library (lib/libqthread_Raspberry_Pi2.a)
1. /opt/qtrpi/bin/qmake-qtrpi -o build/qt_console/Makefile example/qt_console/qt_console.pro -spec devices/linux-rasp-pi2-g++ CONFIG+=release_Raspberry_Pi_2 CONFIG+=qml_debug
2. make -C build/qt_console/
