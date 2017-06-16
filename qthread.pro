QT -= gui

CONFIG += c++11

CONFIG += staticlib
CONFIG += create_prl

TEMPLATE = lib

SOURCES += src/qthread.cpp

HEADERS += include/qthread.hpp

INCLUDEPATH += ./include
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

#(Optional) OpenThread’s diagnostic module library
LIBS += -L/usr/local/src/openthread/output/x86_64-unknown-linux-gnu/lib -lopenthread-diag

DESTDIR = $$PWD/lib
