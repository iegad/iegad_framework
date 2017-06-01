#ifndef __THRIFT_EX_MACRO__
#define __THRIFT_EX_MACRO__


// concurrency include
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
// transport include
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TBufferTransports.h>
// server include
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TNonblockingServer.h>
// protocol include
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>
// boost include
#include <boost/make_shared.hpp>
// stl include
#include <iostream>
#include <ctime>
// iegad tools include
#include "string/iegad_string.hpp"


// 当前 thrift_ex 版本号.
#define VERSION_TYPE "host's version 0.1.3 and thrift's version 0.10.0\n"


// ============================
// @用途 : 事件定义宏
// @__event_type_ : 事件类型名称
// @__delegate_sign_ : 函数签名
// ============================
#define DEFINE_EVENT(__event_type_, __delegate_sign_) \
typedef std::function<##__delegate_sign_##> __event_type_;


// 宏实现定义
#define __XXX_IfFactory__(a)                a##IfFactory
#define __XXX_If__(a)                       a##If
#define __XXX_Handler__(a)                  a##Handler
#define __XXX_ProcessorFactory__(a)         a##ProcessorFactory
#define __XXX_ProcessorCloneFactory__(a)    a##ProcessorCloneFactory
#define __XXX_ProtocolFactory__(a)          apache::thrift::protocol::T##a##ProtocolFactory
#define __XXX_Client__(a)                   a##Client
#define __XXX_Protocol__(a)                 apache::thrift::protocol::T##a##Protocol
#define __XXX_Class_Member(a, b)            a##::##b


// 宏调用声明定义
#define XXX_IfFactory(a)                __XXX_IfFactory__(a)
#define XXX_If(a)                       __XXX_If__(a)
#define XXX_Handler(a)                  __XXX_Handler__(a)
#define XXX_ProcessorFactory(a)         __XXX_ProcessorFactory__(a)	
#define XXX_ProcessorCloneFactory(a)    __XXX_ProcessorCloneFactory__(a)
#define XXX_ProtocolFactory(a)          __XXX_ProtocolFactory__(a)
#define XXX_Client(a)                   __XXX_Client__(a)
#define XXX_Protocol(a	)               __XXX_Protocol__(a)
#define XXX_Class_Member(a, b)          __XXX_Class_Member(a, b)


// 协议定义
#define BINARY_PROTOCOL     Binary
#define COMPACT_PROTOCOL    Compact
#define JSON_PROTOCOL       JSON


// 服务端模型定义
typedef ::apache::thrift::server::TThreadedServer       THRIFT_THREADED_SERVER;
typedef ::apache::thrift::server::TThreadPoolServer     THRIFT_THREADPOOL_SERVER;
typedef ::apache::thrift::server::TNonblockingServer    THRIFT_NON_BLOCKING_SERVER;


// 1分钟超时值
#define _APP_TIME_OUT (1000 * 60) 


#endif // __THRIFT_EX_MACRO__
