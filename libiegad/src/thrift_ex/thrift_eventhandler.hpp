#ifndef __THRIFT_EVENTHANDLER__
#define __THRIFT_EVENTHANDLER__



#include "thrift_ex_macro.h"
#include <thrift/server/TServer.h>
#include <functional>



// ============================
// @用途 : CreateContextEvent事件句柄宏
// @funcName : 函数名称
// @inputProtocol : input协议形参
// @outputProtocol : output协议形参
// @返回值 : void* 为自定义上下文对象的地址
// @PS : 客户端连接时触发
// ============================
#define DEFINE_CREATE_CONTEXT(funcName, inputProtocol, outputProtocol) \
void* funcName(boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol)


// ============================
// @用途 : DeleteContextEvent事件句柄宏
// @funcName : 函数名称
// @serverContext : 给定的上下文环境
// @inputProtocol : input协议形参
// @outputProtocol : output协议形参
// @PS : 客户端关闭通信连接时触发
// ============================
#define DEFINE_DELETE_CONTEXT(funcName, serverContext, inputProtocol, outputProtocol) \
void funcName(void* serverContext, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol)


// ============================
// @用途 : ProcessContextEvent事件句柄宏
// @funcName : 函数名称
// @serverContext : 给定的上下文环境
// @trans : 客户端通信对象
// @PS : 客户端发出RPC调用请求时触发
// ============================
#define DEFINE_PROCESS_CONTEXT(funcName, serverContext, trans) \
void funcName(void* serverContext, boost::shared_ptr<::apache::thrift::transport::TTransport> trans)


// ============================
// @用途 : PreServerEvent事件句柄宏
// @PS : 服务端启动时触发
// ============================
#define DEFINE_PRE_SERVER(funcName) \
void funcName()



// ============================
// @用途 : thirft::TServerEventHandler派生类定义宏
// @className : 派生类类名
// ============================
#define THRIFT_EVENT_HANDLER(className) \
namespace iegad { \
namespace thrift_ex { \
 \
class className : public ::apache::thrift::server::TServerEventHandler { \
public: \
\
	typedef std::function<void*(boost::shared_ptr<::apache::thrift::protocol::TProtocol>, \
	boost::shared_ptr<::apache::thrift::protocol::TProtocol>)> createContextEvent_t; \
\
	typedef std::function<void(void*, \
	boost::shared_ptr<::apache::thrift::protocol::TProtocol>, \
	boost::shared_ptr<::apache::thrift::protocol::TProtocol>)> deleteContextEvent_t; \
\
	typedef std::function<void(void*, \
	boost::shared_ptr<::apache::thrift::transport::TTransport>)> processContextEvent_t; \
\
	typedef std::function<void(void)> preServeEvent_t; \
\
	className () \
	: \
	PreServeEvent(nullptr), \
	CreateContextEvent(nullptr), \
	DeleteContextEvent(nullptr), \
	ProcessContextEvent(nullptr) \
	{} \
\
	virtual ~className() {} \
\
	virtual void* createContext(boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
										boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol) override { \
		if (CreateContextEvent != nullptr) { \
			return CreateContextEvent(inputProtocol, outputProtocol); \
		} \
		return nullptr; \
	} \
	 \
	virtual void deleteContext(void* serverContext, \
										boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
										boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol) override { \
		if (DeleteContextEvent != nullptr) { \
			DeleteContextEvent(serverContext, inputProtocol, outputProtocol); \
		} \
	} \
	 \
	virtual void processContext(void* serverContext, \
										boost::shared_ptr<::apache::thrift::transport::TTransport> trans) override { \
		if (ProcessContextEvent != nullptr) { \
			ProcessContextEvent(serverContext, trans); \
		} \
	} \
\
	virtual void preServe() override { \
		if (PreServeEvent != nullptr) { \
			PreServeEvent(); \
		} \
	} \
 \
public: \
	preServeEvent_t PreServeEvent; \
	createContextEvent_t CreateContextEvent; \
	deleteContextEvent_t DeleteContextEvent; \
	processContextEvent_t ProcessContextEvent; \
}; \
 \
} \
}


// ============================
// @用途 : 服务端事件处理类 定义宏(接口宏, 供用户使用的部分)
// @className : 类名
// ============================
#define DEFINE_EVENT_HANDLER(className) THRIFT_EVENT_HANDLER(className) 


#endif // __THRIFT_EVENTHANDLER__