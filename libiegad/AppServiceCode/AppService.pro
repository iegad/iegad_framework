TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

SOURCES += main.cpp \
    AppService.cpp \
    AppService_constants.cpp \
    AppService_types.cpp \
    AppServiceImp.cpp \
    FuncHandle.cpp \
    Environment.cpp

include(deployment.pri)
qtcAddDeployment()

DISTFILES += \
    AppService.pro.user

HEADERS += \
    iegad_queue.hpp \
    iegad_worker.hpp \
    AppService.h \
    AppService_constants.h \
    AppService_types.h \
    AppServiceImp.h \
    FuncHandle.h \
    Environment.h


LIBS += -L$$PWD/../../../../usr/local/lib/ -lthrift
LIBS += -L$$PWD/../../../../usr/local/lib/ -lglog
LIBS += -L$$PWD/../../tools/acl/lib_acl_cpp/lib/ -l_acl_cpp
LIBS += -L$$PWD/../../tools/acl/lib_acl/lib/ -l_acl
LIBS += -lpthread


INCLUDEPATH += /home/iegad/tools/rapidjson-master/include
INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include
INCLUDEPATH += $$PWD/../../tools/acl/lib_acl/include
DEPENDPATH += $$PWD/../../tools/acl/lib_acl/include
INCLUDEPATH += $$PWD/../../tools/acl/lib_acl_cpp/include
DEPENDPATH += $$PWD/../../tools/acl/lib_acl_cpp/include


PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libthrift.a
PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libglog.a
PRE_TARGETDEPS += $$PWD/../../tools/acl/lib_acl/lib/lib_acl.a
PRE_TARGETDEPS += $$PWD/../../tools/acl/lib_acl_cpp/lib/lib_acl_cpp.a




