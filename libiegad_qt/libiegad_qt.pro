TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
CONFIG += X86_64
CONFIG += console

SOURCES += \
    #../libiegad/test/test_redis.cxx \
    ../libiegad/test/test_string.cxx \
    ../libiegad/test/test_main.cpp \
    ../libiegad/test/test_queue.cxx \
    ../libiegad/test/test_worker.cxx \
    ../libiegad/test/test_timer.cxx \
    #../libiegad/test/test_thrift.cxx \

HEADERS += \
    ../libiegad/iegad/thrift_ex/thrift_client.hpp \
    ../libiegad/iegad/thrift_ex/thrift_ex_macro.h \
    ../libiegad/iegad/thrift_ex/thrift_serializer.hpp \
    ../libiegad/iegad/thrift_ex/thrift_server.hpp \
    ../libiegad/iegad/tools/iegad_log.hpp \
    ../libiegad/iegad/tools/iegad_timer.hpp \
    ../libiegad/iegad/tools/job_queue.hpp \
    ../libiegad/iegad/tools/job_worker.hpp \
    ../libiegad/iegad/string/iegad_string.hpp \
    ../libiegad/iegad/sercurity/iegad_sha1.hpp \
    ../libiegad/iegad/sercurity/iegad_md5.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_command.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_connection.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_exception.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_key.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_client.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_list.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_string.hpp \
    ../libiegad/iegad/net/iegad_tcp_server.hpp \
    ../libiegad/iegad/net/iegad_tcp_session.hpp \
    ../libiegad/iegad/net/iegad_tcp_event.hpp \
    ../libiegad/iegad/net/iegad_ioserive_pool.hpp \
    ../libiegad/iegad/tools/iegad_endian.hpp \
    ../libiegad/iegad/net/iegad_msg_type.hpp \
    ../libiegad/iegad/net/iegad_msg_ctl.hpp \
    ../libiegad/iegad/net/iegad_msg.hpp \
    ../libiegad/iegad/sigar_ex/iegad_sigar.hpp \
    ../libiegad/iegad/sercurity/iegad_aes.hpp \
    ../libiegad/iegad/filesystem/iegad_filesystem.hpp \
    ../libiegad/iegad/iegad_config.h \
    ../libiegad/iegad/iegad.h \
    ../libiegad/iegad/sercurity/iegad_security.hpp \
    ../libiegad/iegad/sercurity/iegad_base64.hpp


INCLUDEPATH += ../libiegad/iegad/ \
    /usr/include\
    /usr/local/include\



LIBS += -L/usr/local/lib -lthrift
LIBS += -L/usr/local/lib -lthriftnb
LIBS += -L/usr/local/lib -lglog
LIBS += -L/usr/local/lib -lgtest
LIBS += -L/usr/local/lib -lhiredis
LIBS += -L/usr/local/lib -lboost_system
LIBS += -L/usr/local/lib -lboost_thread
LIBS += -L/usr/local/lib -lboost_filesystem
LIBS += -L/usr/local/lib -lboost_regex
#LIBS += -L/usr/local/lib -lssl
#LIBS += -L/usr/local/lib -lcrypto
LIBS += -L/usr/local/lib -lsigar

#ubuntu required
#LIBS += -pthread

