TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../libiegad/test/test_thrift_main.cpp \
    ../libiegad/test/test_string.cxx

HEADERS += \
    ../libiegad/src/iegad_define.in.h \
    ../libiegad/src/thrift_ex/thrift_client.hpp \
    ../libiegad/src/thrift_ex/thrift_eventhandler.hpp \
    ../libiegad/src/thrift_ex/thrift_ex_macro.h \
    ../libiegad/src/thrift_ex/thrift_serializer.hpp \
    ../libiegad/src/thrift_ex/thrift_server.hpp \
    ../libiegad/src/tools/iegad_log.hpp \
    ../libiegad/src/tools/iegad_map.hpp \
    ../libiegad/src/tools/iegad_singleton.hpp \
    ../libiegad/src/tools/iegad_timer.hpp \
    ../libiegad/src/tools/job_queue.hpp \
    ../libiegad/src/tools/job_worker.hpp \
    ../libiegad/src/sercurity/iegad_md5.hpp \
    ../libiegad/src/string/iegad_string.hpp \
    ../libiegad/src/sercurity/iegad_aes.hpp \
    ../libiegad/src/sercurity/iegad_sha1.hpp


INCLUDEPATH += ../libiegad/src/ \
    ../libiegad/third_party/include \
    /usr/local/include



unix: LIBS += -L/usr/local/lib -lthrift
unix: LIBS += -L/usr/local/lib -lglog
unix: LIBS += -L/usr/local/lib -lgtest

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../

unix: PRE_TARGETDEPS += /usr/local/lib/libthrift.a
unix: PRE_TARGETDEPS += /usr/local/lib/libglog.a
unix: PRE_TARGETDEPS += /usr/local/lib/libgtest.a
