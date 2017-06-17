QT -= gui

CONFIG += c++11

CONFIG += staticlib
CONFIG += create_prl

TEMPLATE = lib

SOURCES += src/qthread.cpp
HEADERS += include/qthread.hpp

OPENTHREADBASE = /usr/local/src/openthread
OPENTHREADOUT = $$OPENTHREADBASE/output
OPENTHREADLIB = $$OPENTHREADOUT/x86_64-unknown-linux-gnu/lib
OPENTHREADINC = $$OPENTHREADOUT/include

INCLUDEPATH += ./include
INCLUDEPATH += $$OPENTHREADINC

QMAKE_LFLAGS += -Wl,--start-group

#OpenThreads libraries path
LIBS += -L$$OPENTHREADLIB

#OpenThreads Thread Stack implementation of the Full Thread Device (FTD)
LIBS += -lopenthread-ftd

#MbedTLS SSL library with CryptoCell support for ECC operations
LIBS += -lmbedcrypto

#Posix OpenThreads platform implementation library
LIBS += -lopenthread-posix

#OpenThreads platform util which contains the flash manager library
LIBS += -lopenthread-platform-utils

#(Optional) OpenThreads diagnostic module library
LIBS += -lopenthread-diag

for(var, $$list($$member(LIBS, 1, -1))) {
   OPENTHREADINSTALL += $$absolute_path($$replace(var, -l, lib).a, $$OPENTHREADLIB)
}

unix:target.path = /usr/local/lib/qthread
INSTALLS += target

release.depends = staticlib
unix:release.commands = @$(COPY_FILE) $$OPENTHREADINSTALL $(TARGET) $$PWD/lib; cd $$PWD/lib; ls *.a | xargs -n 1 ar x; $(DEL_FILE) *.a; ar csr libqthread_release.a *.o; $(DEL_FILE) *.o; cd -

QMAKE_EXTRA_TARGETS += release
