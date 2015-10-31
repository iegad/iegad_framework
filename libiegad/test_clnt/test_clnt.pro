TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

SOURCES += main.cpp \
    ../src/netc/basic_proxy.cpp \
    ../src/netc/udp_puller.cpp \
    ../src/msg/basic_msg.pb.cc \
    ../src/msg/iegad_io_msg.cpp \
    ../src/common/iegad_log.cpp \
    ../src/common/iegad_md5.cpp \
    ../src/common/iegad_string.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../src/netc/basic_proxy.h \
    ../src/netc/udp_puller.h \
    ../src/msg/basic_msg.pb.h \
    ../src/msg/iegad_io_msg.h \
    ../src/common/iegad_log.h \
    ../src/common/iegad_md5.h \
    ../src/common/iegad_string.h \
    ../src/common/job_queue.hpp \
    ../src/common/job_worker.hpp

DISTFILES += \
    ../src/msg/basic_msg.proto


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lboost_system-gcc48-1_59
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lboost_system-gcc48-1_59
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lboost_system-gcc48-1_59

INCLUDEPATH += $$PWD/../../../../../usr/local/include/boost-1_59
DEPENDPATH += $$PWD/../../../../../usr/local/include/boost-1_59
INCLUDEPATH += ../src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/libboost_system-gcc48-1_59.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/libboost_system-gcc48-1_59.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/boost_system-gcc48-1_59.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/boost_system-gcc48-1_59.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libboost_system-gcc48-1_59.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lprotobuf
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lprotobuf

INCLUDEPATH += $$PWD/../../../../../usr/local/include/google
DEPENDPATH += $$PWD/../../../../../usr/local/include/google

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/libprotobuf.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/libprotobuf.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/protobuf.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/protobuf.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libprotobuf.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lprotoc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lprotoc
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lprotoc

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/libprotoc.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/libprotoc.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/protoc.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/protoc.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libprotoc.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lglog
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lglog
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lglog

INCLUDEPATH += $$PWD/../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../usr/local/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/libglog.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/libglog.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/glog.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/glog.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libglog.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lboost_locale-gcc48-mt-1_59
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lboost_locale-gcc48-mt-1_59
else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lboost_locale-gcc48-mt-1_59

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/libboost_locale-gcc48-mt-1_59.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/libboost_locale-gcc48-mt-1_59.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/boost_locale-gcc48-mt-1_59.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/boost_locale-gcc48-mt-1_59.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libboost_locale-gcc48-mt-1_59.a
