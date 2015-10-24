TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

SOURCES += src/common/iegad_log.cpp \
    src/common/iegad_md5.cpp \
    src/common/iegad_string.cpp \
    src/data/iegad_dbrow.cpp \
    src/data/iegad_dbtab.cpp \
    src/msg/basic_msg.pb.cc \
    src/msg/iegad_io_msg.cpp \
    src/mysql/mysql_helper.cpp \
    src/netc/basic_proxy.cpp \
    src/netc/udp_puller.cpp \
    src/nets/basic_proxy.cpp \
    src/nets/basic_svc.cpp \
    src/nets/tcp_mt_svr.cpp \
    src/nets/udp_puller.cpp \
    src/nets/udp_pusher.cpp \
    test_common/test_common_main.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    src/common/iegad_log.h \
    src/common/iegad_md5.h \
    src/common/iegad_string.h \
    src/common/job_queue.hpp \
    src/common/job_worker.hpp \
    src/data/iegad_dbrow.h \
    src/data/iegad_dbtab.h \
    src/iegad_define.h \
    src/msg/basic_msg.pb.h \
    src/msg/iegad_io_msg.h \
    src/mysql/iegad_mysql.h \
    src/mysql/mysql_helper.h \
    src/netc/basic_proxy.h \
    src/netc/udp_puller.h \
    src/nets/basic_proxy.h \
    src/nets/basic_svc.h \
    src/nets/tcp_mt_svr.h \
    src/nets/udp_puller.h \
    src/nets/udp_pusher.h \
    test_common/job_test.hpp \
    test_common/string_test.hpp

DISTFILES += \
    src/msg/basic_msg.proto \
    src/mysql/iegad_mysql.vcxproj \
    src/mysql/iegad_mysql.vcxproj.filters

