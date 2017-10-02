#Qthread library common configurations

CONFIG(release_Linux_x86_64) {
unix:LIBS += -L$$PWD/lib -lqthread_Linux_x86_64
unix:PRE_TARGETDEPS += $$PWD/lib/libqthread_Linux_x86_64.a
message("Application Release Linux_x86_64: Link libqthread in " $$LIBS)
}else:CONFIG(release_Raspberry_Pi_2) {
unix:LIBS += -L$$PWD/lib -lqthread_Raspberry_Pi2
unix:PRE_TARGETDEPS += $$PWD/lib/libqthread_Raspberry_Pi2.a
message("Application Release Raspberry_Pi_2: Link libqthread in " $$LIBS)
}else:CONFIG(debug_lib) {
unix:LIBS += -L$$OUT_PWD/../qthread -lqthread
unix:PRE_TARGETDEPS += $$OUT_PWD/../qthread/libqthread.a
message("Library Development: Link libqthread in " $$LIBS)
} else: {
unix:LIBS += -L/usr/local/lib/qthread -lqthread
unix:PRE_TARGETDEPS += /usr/local/lib/qthread/libqthread.a
message("Application Development: Link libqthread in " $$LIBS)
}
