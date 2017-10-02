#Qthread library release

for(var, $$list($$member(LIBS, 1, -1))) {
   OPENTHREADINSTALL += $$absolute_path($$replace(var, -l, lib).a, $$OPENTHREADLIB)
}

LOGFILE = openthread.log
COMMITID = $$quote($$system(git -C $$OPENTHREADBASE log -1 --oneline))
write_file($$OUT_PWD/$$LOGFILE, COMMITID)

unix:target.path = /usr/local/lib/qthread
INSTALLS += target

unix:doc.path = /usr/local/lib/qthread
unix:doc.files = $$LOGFILE
INSTALLS += doc

unix:SYSTEM = $$system(uname -s)_$$system(uname -m)

release.depends = staticlib
unix:release.commands = @$(COPY_FILE) $$OPENTHREADINSTALL $(TARGET) $$LOGFILE $$PWD/lib; cd $$PWD/lib; ls *.a | xargs -n 1 ar x; $(DEL_FILE) *.a; ar csr libqthread_$${SYSTEM}.a *.o $$LOGFILE; $(DEL_FILE) *.o $$LOGFILE; cd -

QMAKE_EXTRA_TARGETS += release

release_rpi2.depends = staticlib
unix:release_rpi2.commands = @$(COPY_FILE) $$OPENTHREADINSTALL $(TARGET) $$LOGFILE $$PWD/lib; cd $$PWD/lib; ls *.a | xargs -n 1 ar x; $(DEL_FILE) *.a; ar csr libqthread_Raspberry_Pi2.a *.o $$LOGFILE; $(DEL_FILE) *.o $$LOGFILE; cd -

QMAKE_EXTRA_TARGETS += release_rpi2
