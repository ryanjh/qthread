QT += core
QT -= gui

CONFIG += c++11

TARGET = qt_console
CONFIG += console
CONFIG -= app_bundle

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

HEADERS += \
    qt_console.h

INCLUDEPATH += /usr/local/src/openthread/output/include

QMAKE_LFLAGS += -Wl,--start-group

#OpenThread’s Thread Stack implementation of the Full Thread Device (FTD)
LIBS += -L/usr/local/src/openthread/output/x86_64-unknown-linux-gnu/lib -lopenthread-ftd

#MbedTLS SSL library with CryptoCell support for ECC operations
LIBS += -L/usr/local/src/openthread/output/x86_64-unknown-linux-gnu/lib -lmbedcrypto

#Posix OpenThread’s platform implementation library
LIBS += -L/usr/local/src/openthread/output/x86_64-unknown-linux-gnu/lib -lopenthread-posix

#OpenThread’s platform util which contains the flash manager library
LIBS += -L/usr/local/src/openthread/output/x86_64-unknown-linux-gnu/lib -lopenthread-platform-utils

#(Optional) Command Line Interface (CLI) library
#LIBS += -L/usr/local/src/openthread/output/x86_64-unknown-linux-gnu/lib -lopenthread-cli-ftd

#(Optional) OpenThread’s diagnostic module library
LIBS += -L/usr/local/src/openthread/output/x86_64-unknown-linux-gnu/lib -lopenthread-diag
