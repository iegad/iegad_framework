#ifndef __THRIFT_SERVER__
#define __THRIFT_SERVER__



// ============ 说明 ============
//
// @创建日期 : 2016-02-23
// @创建人 : iegad
//
// ============================
// @用途 : 1, 对thrift 三种Server模型的扩展
//				2, 提供快速开发
//		
// @PS : 该文件依赖于 thrift 开源库    
//
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================
// 2016-03-04		    -- iegad		1, 去掉 构造函数中的 threadCount的默认值( default value = 4).
//								2, 增加 ThreadServer 模型, 通过 重载构造函数的方式实现.
//								3, 将 server_ 类型 由 ThreadPoolServer 替换为 TServerFramework, 为了实现多态.
// 2016-04-19		    -- iegad		1, 去掉服务器包的概念, 改为用宏定义.
//								2, 添加NonBlockingServer和ThreadedServer两种不同的服务器模型
//								3, 去掉InitEnvironment静态函数
// 2016-05-06		    -- iegad		1, 改用宏来定义服务端



#include "thrift_ex_macro.h"
#include "thrift_eventhandler.hpp"
#include "tools/iegad_log.hpp"



// ============================
// @用途 : socket资源初始化
// @返回值 : 初始化成功返回true, 否则返回false
// @PS : 只在WINDOWS平台有效
// ============================
static inline bool
socket_init()
{
#ifdef WIN32
	WSADATA wData;
	if (WSAStartup(0x0202, &wData) != 0) {
		return false;
	}
	return HIBYTE(wData.wVersion) == 2 && LOBYTE(wData.wVersion) == 2;
#endif // WIN32
	return true;
}


// ============================
// @用途 : socket资源回收
// @返回值 : void
// @PS : 只在WINDOWS平台有效
// ============================
static inline void
socket_release()
{
#ifdef WIN32
	WSACleanup();
#endif // WIN32
}



// ============================
// @用途 : 处理机工厂定义宏
// @serviceIfFactory : RPC接口工厂
// @serviceIf : RPC接口
// @serviceHandler : RPC接口实现句柄类
// ============================
#define DEFINE_PROCESSOR_FACTORY(serviceIfFactory, serviceIf, serviceHandler) \
namespace iegad { \
namespace thrift_ex { \
 \
class ProcessorCloneFactory : virtual public serviceIfFactory { \
public: \
	virtual ~ProcessorCloneFactory() {} \
		\
	virtual serviceIf * getHandler(const ::apache::thrift::TConnectionInfo & connInfo) { \
		boost::shared_ptr<::apache::thrift::transport::TSocket> sock = \
		boost::dynamic_pointer_cast<::apache::thrift::transport::TSocket>(connInfo.transport); \
		sock->setRecvTimeout(_APP_TIME_OUT); \
		return new serviceHandler; \
	} \
	\
	virtual void releaseHandler(serviceIf * handler) { \
		delete handler; \
	} \
}; \
} \
}


// ============================
// @用途 : per-connect 模式服务端定义宏
// @serviceName : 服务名称
// @serviceIfFactory : RPC接口工厂
// @serviceIf : RPC接口
// @serviceHandler : RPC接口实现句柄类
// @serviceProcessorFactory : 处理机工厂
// @protocol_type : 协议类型; 支持的协议有 Json, binary, compact
// ============================
#define THRIFT_THREADED_SERVICE(serviceName, serviceIfFactory, serviceIf, serviceHandler, serviceProcessorFactory, protocol_type) \
DEFINE_PROCESSOR_FACTORY(serviceIfFactory, serviceIf, serviceHandler) \
namespace iegad { \
namespace thrift_ex { \
\
class serviceName { \
public: \
	typedef ::apache::thrift::server::TServerEventHandler TServerEventHandler; \
	typedef protocol_type protoc_fac_t;	\
	typedef ::apache::thrift::server::TThreadedServer	TThreadedServer; \
	\
	explicit serviceName(int port, \
	boost::shared_ptr<TServerEventHandler> eventHandler = nullptr, \
	int threadCount = 4) \
		: \
	threadCount_(threadCount), \
	port_(port), \
	server_(nullptr) { \
		this->_init(eventHandler);	\
		} \
	\
	~serviceName() {	\
		server_->stop(); \
		} \
	\
	void Run() { \
		iINFO << "===============================\n"; \
		iINFO << VERSION_TYPE << std::endl; \
		iINFO << "Threaded Server running : listened at 0.0.0.0 : " << port_ << std::endl; \
		iINFO << "===============================\n\n"; \
		server_->run(); \
	} \
	\
	boost::shared_ptr<TThreadedServer> GetServer() { \
		return this->server_;	 \
	} \
	\
private: \
	void _init(boost::shared_ptr<TServerEventHandler> eventHandler) {	\
		using ::apache::thrift::concurrency::PlatformThreadFactory; \
		using ::apache::thrift::transport::TServerSocket;	\
		using ::apache::thrift::transport::TBufferedTransportFactory; \
		using ::apache::thrift::server::TThreadedServer; \
		using ::apache::thrift::protocol::TBinaryProtocolFactory; \
		using ::apache::thrift::protocol::TJSONProtocolFactory; \
		using ::apache::thrift::protocol::TCompactProtocolFactory; \
		server_ = boost::shared_ptr<TThreadedServer>(new TThreadedServer( \
			boost::make_shared<serviceProcessorFactory>(boost::make_shared<ProcessorCloneFactory>()), \
			boost::make_shared<TServerSocket>(port_), \
			boost::make_shared<TBufferedTransportFactory>(),	\
			boost::make_shared<protoc_fac_t>())); \
		server_->setConcurrentClientLimit(threadCount_); \
		if (eventHandler != nullptr) { \
			server_->setServerEventHandler(eventHandler); \
				} \
		} \
	\
	int port_; \
	int threadCount_; \
	boost::shared_ptr<::apache::thrift::server::TThreadedServer> server_;	 \
	\
	serviceName(const serviceName &); \
	serviceName & operator=(const serviceName &); \
}; \
} \
}


// ============================
// @用途 : connect-pool 模式服务端定义宏
// @serviceName : 服务名称
// @serviceIfFactory : RPC接口工厂
// @serviceIf : RPC接口
// @serviceHandler : RPC接口实现句柄类
// @serviceProcessorFactory : 处理机工厂
// @protocol_type : 协议类型; 支持的协议有 Json, binary, compact
// ============================
#define THRIFT_THREAD_POOL_SERVICE(serviceName, serviceIfFactory, serviceIf, serviceHandler, serviceProcessorFactory, protocol_type) \
DEFINE_PROCESSOR_FACTORY(serviceIfFactory, serviceIf, serviceHandler) \
namespace iegad { \
namespace thrift_ex { \
\
class serviceName { \
public: \
	typedef ::apache::thrift::server::TServerEventHandler TServerEventHandler; \
	typedef protocol_type protoc_fac_t;	\
	typedef ::apache::thrift::server::TThreadedServer	TThreadedServer; \
	\
	explicit serviceName(int port, \
	boost::shared_ptr<TServerEventHandler> eventHandler = nullptr, \
	int threadCount = 4) \
		: \
	threadCount_(threadCount), \
	port_(port), \
	server_(nullptr) { \
		this->_init(eventHandler);	\
	} \
	\
	~serviceName() {	\
		server_->stop(); \
	} \
	\
	void Run() { \
		iINFO << "===============================\n"; \
		iINFO << VERSION_TYPE << std::endl; \
		iINFO << "ThreadPool Server running : listened at 0.0.0.0 : " << port_ << std::endl; \
		iINFO << "===============================\n\n"; \
		server_->run(); \
		} \
	\
	boost::shared_ptr<TThreadPoolServer> GetServer() { \
		return this->server_;	 \
		} \
	\
private: \
	void _init(boost::shared_ptr<TServerEventHandler> eventHandler) {	\
		using ::apache::thrift::concurrency::ThreadManager; \
		using ::apache::thrift::concurrency::PlatformThreadFactory; \
		using ::apache::thrift::transport::TServerSocket;	\
		using ::apache::thrift::transport::TBufferedTransportFactory; \
		using ::apache::thrift::protocol::TBinaryProtocolFactory; \
		using ::apache::thrift::protocol::TJSONProtocolFactory; \
		using ::apache::thrift::protocol::TCompactProtocolFactory; \
		threadManager_ = ThreadManager::newSimpleThreadManager(threadCount_); \
		threadManager_->threadFactory(boost::shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory())); \
		threadManager_->start(); \
		server_ = boost::shared_ptr<TThreadPoolServer>(new TThreadPoolServer( \
			boost::make_shared<serviceProcessorFactory>(boost::make_shared<ProcessorCloneFactory>()), \
			boost::make_shared<TServerSocket>(port_), \
			boost::make_shared<TBufferedTransportFactory>(),	\
			boost::make_shared<protoc_fac_t>(), \
			threadManager_)); \
		if (eventHandler != nullptr) { \
			server_->setServerEventHandler(eventHandler); \
		} \
	} \
	\
	int port_; \
	int threadCount_; \
	boost::shared_ptr<::apache::thrift::concurrency::ThreadManager> threadManager_; \
	boost::shared_ptr<::apache::thrift::server::TThreadPoolServer> server_; \
	\
	serviceName(const serviceName &); \
	serviceName & operator=(const serviceName &); \
}; \
} \
}


// ============================
// @用途 : IO复用+非阻塞套接字 模式服务端定义宏
// @serviceName : 服务名称
// @serviceIfFactory : RPC接口工厂
// @serviceIf : RPC接口
// @serviceHandler : RPC接口实现句柄类
// @serviceProcessorFactory : 处理机工厂
// @protocol_type : 协议类型; 支持的协议有 Json, binary, compact
// ============================
#define THRIFT_NON_BLOCKING_SERVICE(serviceName, serviceIfFactory, serviceIf, serviceHandler, serviceProcessorFactory, protocol_type) \
DEFINE_PROCESSOR_FACTORY(serviceIfFactory, serviceIf, serviceHandler) \
namespace iegad { \
namespace thrift_ex { \
\
class serviceName { \
public: \
	typedef ::apache::thrift::server::TServerEventHandler TServerEventHandler; \
	typedef protocol_type						protoc_fac_t; \
	typedef ::apache::thrift::server::TNonblockingServer	TNonblockingServer; \
	\
	explicit serviceName(int port, \
	boost::shared_ptr<TServerEventHandler> eventHandler = nullptr, \
	int threadCount = 4) \
		: \
	threadCount_(threadCount), \
	port_(port), \
	server_(nullptr) { \
		this->_init(eventHandler);	\
		} \
	\
	~serviceName() {	\
		socket_release(); \
		server_->stop(); \
	} \
	\
	void Run() { \
		iINFO << "===============================\n"; \
		iINFO << VERSION_TYPE << std::endl; \
		iINFO << "Nonblocking Server running : listened at 0.0.0.0 : " << port_ << std::endl; \
		iINFO << "===============================\n\n"; \
		server_->run(); \
		} \
	\
	boost::shared_ptr<TNonblockingServer> GetServer() { \
		return this->server_;	 \
		} \
	\
private: \
	void _init(boost::shared_ptr<TServerEventHandler> eventHandler) {	\
		using ::apache::thrift::concurrency::ThreadManager; \
		using ::apache::thrift::concurrency::PlatformThreadFactory; \
		using ::apache::thrift::transport::TServerSocket;	\
		using ::apache::thrift::transport::TBufferedTransportFactory; \
		threadManager_ = ThreadManager::newSimpleThreadManager(threadCount_); \
		threadManager_->threadFactory(boost::shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory())); \
		threadManager_->start(); \
		server_ = boost::shared_ptr<TNonblockingServer>(new TNonblockingServer( \
			boost::make_shared<serviceProcessorFactory>(boost::make_shared<ProcessorCloneFactory>()), \
			boost::make_shared<TBufferedTransportFactory>(), \
			boost::make_shared<TBufferedTransportFactory>(), \
			boost::make_shared<protoc_fac_t>(), \
			boost::make_shared<protoc_fac_t>(), \
			port_, \
			threadManager_)); \
		if (eventHandler != nullptr) { \
			if (!socket_init()) { \
				throw std::exception("Windows socket init failed"); \
			} \
			server_->setServerEventHandler(eventHandler); \
		} \
	} \
	\
	int port_; \
	int threadCount_; \
	boost::shared_ptr<::apache::thrift::concurrency::ThreadManager> threadManager_; \
	boost::shared_ptr<::apache::thrift::server::TNonblockingServer> server_; \
	\
	serviceName(const serviceName &); \
	serviceName & operator=(const serviceName &); \
}; \
} \
}

// ============================
// @用途 : per-connect 模式服务端定义宏(接口宏, 供用户使用的部分)
// @serviceName : 服务名称
// @__protocol : 协议类型; 支持的协议有 Json, binary, compact
// ============================
#define DEFINE_THREADED_SERVER(serviceName, __protocol) THRIFT_THREADED_SERVICE(serviceName, \
XXX_IfFactory(serviceName), \
XXX_If(serviceName), \
XXX_Handler(serviceName), \
XXX_ProcessorFactory(serviceName), \
XXX_ProtocolFactory(__protocol))


// ============================
// @用途 : connect-pool 模式服务端定义宏(接口宏, 供用户使用的部分)
// @serviceName : 服务名称
// @__protocol : 协议类型; 支持的协议有 Json, binary, compact
// ============================
#define DEFINE_THREADPOOL_SERVER(serviceName, __protocol) THRIFT_THREAD_POOL_SERVICE(serviceName, \
XXX_IfFactory(serviceName), \
XXX_If(serviceName), \
XXX_Handler(serviceName), \
XXX_ProcessorFactory(serviceName), \
XXX_ProtocolFactory(__protocol))


// ============================
// @用途 : IO复用+非阻塞套接字 模式服务端定义宏(接口宏, 供用户使用的部分)
// @serviceName : 服务名称
// @__protocol : 协议类型; 支持的协议有 Json, binary, compact
// ============================
#define DEFINE_NON_BLOCKING_SERVER(serviceName, __protocol) THRIFT_NON_BLOCKING_SERVICE(serviceName, \
XXX_IfFactory(serviceName), \
XXX_If(serviceName), \
XXX_Handler(serviceName), \
XXX_ProcessorFactory(serviceName), \
XXX_ProtocolFactory(__protocol))


#endif // __THRIFT_SERVER__