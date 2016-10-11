TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += \
    #../libiegad/test/test_redis.cxx \
    ../libiegad/test/test_string.cxx \
    ../libiegad/test/test_main.cpp \
    ../libiegad/test/test_queue.cxx \
    ../libiegad/test/test_worker.cxx \
    ../libiegad/test/test_timer.cxx \    
    ../libiegad/test/gen-cpp/EchoServer.cpp \
    ../libiegad/test/gen-cpp/testing_constants.cpp \
    ../libiegad/test/gen-cpp/testing_types.cpp \
    ../libiegad/test/gen-cpp/TimeServer.cpp \
    #../libiegad/test/test_thrift.cxx \
    ../libiegad/iegad/rpc/iegad_rpc_constants.cpp \
    ../libiegad/iegad/rpc/iegad_rpc_types.cpp \
    ../libiegad/iegad/rpc/iegadService.cpp

HEADERS += \
    ../libiegad/iegad/thrift_ex/thrift_client.hpp \
    ../libiegad/iegad/thrift_ex/thrift_ex_macro.h \
    ../libiegad/iegad/thrift_ex/thrift_serializer.hpp \
    ../libiegad/iegad/thrift_ex/thrift_server.hpp \
    ../libiegad/iegad/tools/iegad_log.hpp \
    ../libiegad/iegad/tools/iegad_timer.hpp \
    ../libiegad/iegad/tools/job_queue.hpp \
    ../libiegad/iegad/tools/job_worker.hpp \
    ../libiegad/iegad/sercurity/iegad_md5.hpp \
    ../libiegad/iegad/string/iegad_string.hpp \
    ../libiegad/iegad/sercurity/iegad_sha1.hpp \
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
    ../libiegad/iegad/rpc/iegad_msg_id.h \
    ../libiegad/iegad/rpc/iegad_rpc_constants.h \
    ../libiegad/iegad/rpc/iegad_rpc_types.h \
    ../libiegad/iegad/rpc/iegadService_server.skeleton.hpp \
    ../libiegad/iegad/rpc/iegad_basic_handler.hpp


INCLUDEPATH += ../libiegad/iegad/ \
    ../iegad/third_party/include \
    /usr/local/include



LIBS += -L/usr/local/lib -lthrift
LIBS += -L/usr/local/lib -lthriftnb
LIBS += -L/usr/local/lib -lglog
LIBS += -L/usr/local/lib -lgtest
LIBS += -L/usr/local/lib -lhiredis
LIBS += -L/usr/local/lib -lboost_system
LIBS += -L/usr/local/lib -lboost_thread

#ubuntu required
LIBS += -pthread

DISTFILES += \
    ../libiegad/iegad/rpc/iegad_rpc.thrift

