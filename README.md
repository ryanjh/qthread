# Qthread = Qt + OpenThread
![Architecture](https://cloud.githubusercontent.com/assets/22163926/26624410/59df27ca-4623-11e7-8ea7-15b68b3f2f71.JPG)

Build (qt_console):
1. mkdir ~/build
2. cd ~/build
3. /opt/Qt/5.8/gcc_64/bin/qmake -o Makefile ../qt_console/qt_console.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
4. make
