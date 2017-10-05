#Qthread library release

unix:SYSTEM = $$system(uname -s)_$$system(uname -m)

unix:RELEASE_SCRIPT = $(MKDIR) $$PWD/lib/tmp; $(COPY_FILE) $$OPENTHREADINSTALL $(TARGET) $$LOGFILE $$PWD/lib/tmp; cd $$PWD/lib/tmp; ls *.a | xargs -n 1 ar x; ar csr libqthread_release.a *.o $$LOGFILE; cd -
unix:RELEASE_CLEAN = $(DEL_FILE) -r $$PWD/lib/tmp

release.depends = staticlib
unix:release.commands = @$$RELEASE_SCRIPT; $(MOVE) $$PWD/lib/tmp/libqthread_release.a $$PWD/lib/libqthread_$${SYSTEM}.a; $$RELEASE_CLEAN
QMAKE_EXTRA_TARGETS += release

release_rpi2.depends = staticlib
unix:release_rpi2.commands = @$$RELEASE_SCRIPT; $(MOVE) $$PWD/lib/tmp/libqthread_release.a $$PWD/lib/libqthread_Raspberry_Pi2.a; $$RELEASE_CLEAN
QMAKE_EXTRA_TARGETS += release_rpi2
