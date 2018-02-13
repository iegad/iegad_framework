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
    ../libiegad/test/test_tools.cxx \
    ../libiegad/test/test_dai.cxx \
    ../libiegad/test/test_tcp.cxx

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
    ../libiegad/iegad/security/iegad_md5.hpp \
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
    ../libiegad/iegad/security/iegad_aes.hpp \
    ../libiegad/iegad/filesystem/iegad_filesystem.hpp \
    ../libiegad/iegad/security/iegad_security.hpp \
    ../libiegad/iegad/security/iegad_base64.hpp \
    ../libiegad/iegad/zmq_ex/zmq_publisher.hpp \
    ../libiegad/iegad/net/iegad_email.hpp \
    ../libiegad/iegad/zmq_ex/zmq_subscriber.hpp \
    ../libiegad/iegad/tigen/iegad_tigen_base_handler.hpp \
    ../libiegad/iegad/dai/iegad_mysql++.hpp \
    ../libiegad/iegad/dai/iegad_mongo.hpp \
    ../libiegad/iegad/net/tcp_server.hpp \
    ../libiegad/iegad/net/tcp_session.hpp \
    ../libiegad/iegad/net/tcp_macro.h


INCLUDEPATH += ../libiegad/ \
    /usr/include \
    /usr/local/include \
    /usr/local/mysql/include \
    /usr/local/include/mongocxx/v_noabi/ \
    /usr/local/include/bsoncxx/v_noabi/ \



LIBS += -L/usr/local/lib -lthrift \
                         -lthriftnb \
                         -lglog \
                         -lgtest \
                         -lhiredis \
                         -lboost_system \
                         -lboost_thread \
                         -lboost_filesystem \
                         -lboost_regex \
                         -lssl \
                         -lcrypto \
                         -lsigar \
                         -lmysqlpp \
                         -ljsoncpp \
                         -lbsoncxx \
                         -lmongocxx \
                         -levent

#ubuntu required
#LIBS += -pthread

