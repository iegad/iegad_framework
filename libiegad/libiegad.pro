TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

SOURCES += src/common/iegad_log.cpp \
    src/common/iegad_md5.cpp \
    src/common/iegad_string.cpp \
    test_common/test_common_main.cpp

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
    src/iegad_define.h

INCLUDEPATH += ./src


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
