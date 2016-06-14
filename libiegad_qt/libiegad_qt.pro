TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../src/test/test_string.cxx \
    ../src/test/test_rmq.cxx \
    ../src/test/test_redis.cxx \
    ../src/test/test_thrift.cxx

HEADERS += \
    ../src/iegad/iegad_define.in.h \
    ../src/iegad/thrift_ex/thrift_client.hpp \
    ../src/iegad/thrift_ex/thrift_eventhandler.hpp \
    ../src/iegad/thrift_ex/thrift_ex_macro.h \
    ../src/iegad/thrift_ex/thrift_serializer.hpp \
    ../src/iegad/thrift_ex/thrift_server.hpp \
    ../src/iegad/tools/iegad_log.hpp \
    ../src/iegad/tools/iegad_map.hpp \
    ../src/iegad/tools/iegad_singleton.hpp \
    ../src/iegad/tools/iegad_timer.hpp \
    ../src/iegad/tools/job_queue.hpp \
    ../src/iegad/tools/job_worker.hpp \
    ../src/iegad/sercurity/iegad_md5.hpp \
    ../src/iegad/string/iegad_string.hpp \
    ../src/iegad/sercurity/iegad_aes.hpp \
    ../src/iegad/sercurity/iegad_sha1.hpp \
    ../src/iegad/rabbitmq_ex/rabbitmq_queue.hpp \
    ../src/iegad/rabbitmq_ex/rabbitmq_producter.hpp \
    ../src/iegad/rabbitmq_ex/rabbitmq_session.hpp \
    ../src/iegad/rabbitmq_ex/rabbitmq_consumer.hpp \
    ../src/iegad/rabbitmq_ex/rabbitmq_publisher.hpp \
    ../src/iegad/rabbitmq_ex/rabbitmq_subcriber.hpp \
    ../src/iegad/rabbitmq_ex/rabbitmq_exchange.hpp \
    ../src/iegad/rabbitmq_ex/rabbitmq_sender.hpp \
    ../src/iegad/rabbitmq_ex/wwww.hpp \
    ../src/iegad/rabbitmq_ex/rabbitmq_basic.hpp \
    ../src/iegad/rabbitmq_ex/rabbitmq_recver.hpp \
    ../src/iegad/redis_ex/iegad_redis_command.hpp \
    ../src/iegad/redis_ex/iegad_redis_connection.hpp \
    ../src/iegad/redis_ex/iegad_redis_exception.hpp \
    ../src/iegad/redis_ex/iegad_redis_key.hpp


INCLUDEPATH += ../src/iegad/ \
    ../iegad/third_party/include \
    /usr/local/include



unix: LIBS += -L/usr/local/lib -lthrift
unix: LIBS += -L/usr/local/lib -lglog
unix: LIBS += -L/usr/local/lib -lgtest
unix: LIBS += -L/usr/local/lib -lSimpleAmqpClient.2.4.0
unix: LIBS += -L/usr/local/lib -lhiredis


