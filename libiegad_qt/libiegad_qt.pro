TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    ../libiegad/test/test_thrift_main.cpp \
    ../libiegad/test/test_string.cxx \
    ../libiegad/test/test_rmq.cxx

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
    ../libiegad/src/sercurity/iegad_sha1.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_queue.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_producter.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_session.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_consumer.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_publisher.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_subcriber.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_exchange.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_interface副本 2.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_interface副本 3.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_interface副本 3.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_sender.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_interface副本 2.hpp \
    ../libiegad/src/rabbitmq_ex/wwww.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_basic.hpp \
    ../libiegad/src/rabbitmq_ex/rabbitmq_recver.hpp


INCLUDEPATH += ../libiegad/src/ \
    ../libiegad/third_party/include \
    /usr/local/include



unix: LIBS += -L/usr/local/lib -lthrift
unix: LIBS += -L/usr/local/lib -lglog
unix: LIBS += -L/usr/local/lib -lgtest
unix: LIBS += -L/usr/local/lib -lSimpleAmqpClient.2.4.0


