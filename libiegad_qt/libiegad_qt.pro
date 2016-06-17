TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += \
    #../libiegad/test/test_rmq.cxx \
    #../libiegad/test/test_redis.cxx \
    #../libiegad/test/test_thrift.cxx \
    ../libiegad/test/test_string.cxx

HEADERS += \
    ../libiegad/iegad/iegad_define.in.h \
    ../libiegad/iegad/thrift_ex/thrift_client.hpp \
    ../libiegad/iegad/thrift_ex/thrift_eventhandler.hpp \
    ../libiegad/iegad/thrift_ex/thrift_ex_macro.h \
    ../libiegad/iegad/thrift_ex/thrift_serializer.hpp \
    ../libiegad/iegad/thrift_ex/thrift_server.hpp \
    ../libiegad/iegad/tools/iegad_log.hpp \
    ../libiegad/iegad/tools/iegad_map.hpp \
    ../libiegad/iegad/tools/iegad_singleton.hpp \
    ../libiegad/iegad/tools/iegad_timer.hpp \
    ../libiegad/iegad/tools/job_queue.hpp \
    ../libiegad/iegad/tools/job_worker.hpp \
    ../libiegad/iegad/sercurity/iegad_md5.hpp \
    ../libiegad/iegad/string/iegad_string.hpp \
    ../libiegad/iegad/sercurity/iegad_aes.hpp \
    ../libiegad/iegad/sercurity/iegad_sha1.hpp \
    ../libiegad/iegad/rabbitmq_ex/rabbitmq_queue.hpp \
    ../libiegad/iegad/rabbitmq_ex/rabbitmq_producter.hpp \
    ../libiegad/iegad/rabbitmq_ex/rabbitmq_session.hpp \
    ../libiegad/iegad/rabbitmq_ex/rabbitmq_consumer.hpp \
    ../libiegad/iegad/rabbitmq_ex/rabbitmq_publisher.hpp \
    ../libiegad/iegad/rabbitmq_ex/rabbitmq_subcriber.hpp \
    ../libiegad/iegad/rabbitmq_ex/rabbitmq_exchange.hpp \
    ../libiegad/iegad/rabbitmq_ex/rabbitmq_sender.hpp \
    ../libiegad/iegad/rabbitmq_ex/wwww.hpp \
    ../libiegad/iegad/rabbitmq_ex/rabbitmq_basic.hpp \
    ../libiegad/iegad/rabbitmq_ex/rabbitmq_recver.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_command.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_connection.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_exception.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_key.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_client.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_list.hpp \
    ../libiegad/iegad/redis_ex/iegad_redis_string.hpp


INCLUDEPATH += ../libiegad/iegad/ \
    ../iegad/third_party/include \
    /usr/local/include



unix: LIBS += -L/usr/local/lib -lthrift
unix: LIBS += -L/usr/local/lib -lglog
unix: LIBS += -L/usr/local/lib -lgtest
unix: LIBS += -L/usr/local/lib -lSimpleAmqpClient
unix: LIBS += -L/usr/local/lib -lhiredis
#ubuntu required
unix: LIBS += -pthread
