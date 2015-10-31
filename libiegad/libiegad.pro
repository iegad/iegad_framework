TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

SOURCES += src/common/iegad_log.cpp \
    src/common/iegad_md5.cpp \
    src/common/iegad_string.cpp \
    #test_common/test_common_main.cpp \
    src/sqlite/sqlite_helper.cpp \
    src/sqlite/sqlite3.c \
    src/data/iegad_dbrow.cpp \
    src/data/iegad_dbtab.cpp \
    src/nets/basic_proxy.cpp \
    src/nets/basic_svc.cpp \
    src/nets/tcp_mt_svr.cpp \
    src/nets/udp_puller.cpp \
    src/nets/udp_pusher.cpp \
    src/msg/basic_msg.pb.cc \
    src/msg/iegad_io_msg.cpp \
    test_server/test_server_main.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    src/common/iegad_log.h \
    src/common/iegad_md5.h \
    src/common/iegad_string.h \
    src/common/job_queue.hpp \
    src/common/job_worker.hpp \
    test_common/job_test.hpp \
    test_common/string_test.hpp \
    src/iegad_define.h \
    test_common/log_test.hpp \
    src/sqlite/iegad_sqlite.h \
    src/sqlite/sqlite3.h \
    src/sqlite/sqlite3ext.h \
    src/sqlite/sqlite_helper.h \
    src/data/iegad_dbrow.h \
    src/data/iegad_dbtab.h \
    src/nets/basic_proxy.h \
    src/nets/basic_svc.h \
    src/nets/tcp_mt_svr.h \
    src/nets/udp_puller.h \
    src/nets/udp_pusher.h \
    src/msg/basic_msg.pb.h \
    src/msg/iegad_io_msg.h \
    test_server/echo_svc.hpp

INCLUDEPATH += ./src
LIBS += -l dl


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lboost_system-gcc48-mt-1_59
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lboost_system-gcc48-mt-1_59
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lboost_system-gcc48-mt-1_59

INCLUDEPATH += $$PWD/../../../../usr/local/include/boost-1_59
DEPENDPATH += $$PWD/../../../../usr/local/include/boost-1_59

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/libboost_system-gcc48-mt-1_59.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/libboost_system-gcc48-mt-1_59.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/boost_system-gcc48-mt-1_59.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/boost_system-gcc48-mt-1_59.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libboost_system-gcc48-mt-1_59.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lglog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lglog
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lglog

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/libglog.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/libglog.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/glog.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/glog.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libglog.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lgtest
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lgtest
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lgtest

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/libgtest.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/libgtest.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/gtest.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/gtest.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libgtest.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lboost_system-gcc48-mt-1_59
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lboost_system-gcc48-mt-1_59
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lboost_system-gcc48-mt-1_59

INCLUDEPATH += $$PWD/../../../../usr/local/include/boost-1_59
DEPENDPATH += $$PWD/../../../../usr/local/include/boost-1_59

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/libboost_system-gcc48-mt-1_59.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/libboost_system-gcc48-mt-1_59.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/boost_system-gcc48-mt-1_59.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/boost_system-gcc48-mt-1_59.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libboost_system-gcc48-mt-1_59.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lboost_locale-gcc48-mt-1_59
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lboost_locale-gcc48-mt-1_59
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lboost_locale-gcc48-mt-1_59

INCLUDEPATH += $$PWD/../../../../usr/local/include/boost-1_59
DEPENDPATH += $$PWD/../../../../usr/local/include/boost-1_59

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/libboost_locale-gcc48-mt-1_59.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/libboost_locale-gcc48-mt-1_59.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/boost_locale-gcc48-mt-1_59.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/boost_locale-gcc48-mt-1_59.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libboost_locale-gcc48-mt-1_59.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lglog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lglog
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lglog

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/libglog.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/libglog.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/glog.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/glog.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libglog.a

DISTFILES += \
    src/msg/basic_msg.proto

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lprotobuf
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lprotobuf

INCLUDEPATH += $$PWD/../../../../usr/local/include/google
DEPENDPATH += $$PWD/../../../../usr/local/include/google

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/libprotobuf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/libprotobuf.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/protobuf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/protobuf.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libprotobuf.a
