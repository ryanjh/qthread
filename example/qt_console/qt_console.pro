QT += core
QT -= gui

CONFIG += c++11

TARGET = qt_console
CONFIG += console
CONFIG -= app_bundle
CONFIG += link_prl

TEMPLATE = app

SOURCES += main.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += qt_console.h

INCLUDEPATH += $$PWD/../../include

CONFIG(test_Linux_x86_64) {
unix:LIBS += -L$$PWD/../../lib -lqthread_Linux_x86_64
unix:PRE_TARGETDEPS += $$PWD/../../lib/libqthread_Linux_x86_64.a
message("Application Release Linux_x86_64: Link libqthread in " $$LIBS)
}else:CONFIG(test) {
unix:LIBS += -L$$OUT_PWD/../qthread -lqthread
unix:PRE_TARGETDEPS += $$OUT_PWD/../qthread/libqthread.a
message("Library Development: Link libqthread in " $$LIBS)
} else {
unix:LIBS += -L/usr/local/lib/qthread -lqthread
unix:PRE_TARGETDEPS += /usr/local/lib/qthread/libqthread.a
message("Application Development: Link libqthread in " $$LIBS)
}
