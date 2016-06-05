TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../libiegad/test/test_string_main.cpp \
    ../libiegad/test/test_thrift_main.cpp

HEADERS += \
    ../libiegad/src/iegad_common.h \
    ../libiegad/src/iegad_common.in.h \
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
    ../libiegad/src/string/iegad_string.hpp


INCLUDEPATH += ../libiegad/src/ \
    ../libiegad/third_party/include \
    /usr/local/include


