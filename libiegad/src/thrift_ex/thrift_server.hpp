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



#define THRIFT_SERVICE(serviceIfFactory, serviceIf, serviceHandler, serviceProcessorFactory, protocol_type) \
namespace iegad { \
namespace thrift_ex { \
class ProcessorCloneFactory : virtual public serviceIfFactory \
{ \
public: \
	virtual ~ProcessorCloneFactory() {} \
	virtual serviceIf * getHandler(const ::apache::thrift::TConnectionInfo & connInfo) { \
		boost::shared_ptr<::apache::thrift::transport::TSocket> sock = \
		boost::dynamic_pointer_cast<::apache::thrift::transport::TSocket>(connInfo.transport); \
		sock->setRecvTimeout(_APP_TIME_OUT); \
		return new serviceHandler; \
	} \
	virtual void releaseHandler(serviceIf * handler) { \
		delete handler; \
	} \
}; \
class THost {	\
public: \
	typedef ::apache::thrift::server::TServerEventHandler TServerEventHandler; \
	typedef protocol_type protoc_fac_t;	\
	typedef ::apache::thrift::server::TThreadedServer	TThreadedServer;	\
	explicit THost(int port, \
	boost::shared_ptr<TServerEventHandler> eventHandler = nullptr, \
	int threadCount = 4) \
		: \
	threadCount_(threadCount),	\
	port_(port), \
	server_(nullptr) { \
		this->_init(eventHandler);	\
	} \
	~THost() {	\
		server_->stop(); \
	} \
	void Run() { \
		std::cout << "===============================\n"; \
		std::cout << VERSION_TYPE << std::endl; \
		std::cout << "Server running...\nlistened at 0.0.0.0 : " << port_ << std::endl; \
		std::cout << "===============================\n\n"; \
		server_->run(); \
	} \
	boost::shared_ptr<TThreadedServer> GetServer() { \
		return this->server_;	 \
	} \
private:	\
	void _init(boost::shared_ptr<TServerEventHandler> eventHandler) {	\
		using ::apache::thrift::concurrency::ThreadManager; \
		using ::apache::thrift::concurrency::PlatformThreadFactory; \
		using ::apache::thrift::transport::TServerSocket;	\
		using ::apache::thrift::transport::TBufferedTransportFactory; \
		using ::apache::thrift::transport::TSSLSocket;	\
		using ::apache::thrift::transport::TSSLSocketFactory; \
		using ::apache::thrift::transport::TSSLServerSocket; \
		using ::apache::thrift::server::TThreadedServer; \
		using ::apache::thrift::protocol::TBinaryProtocolFactory;	\
		using ::apache::thrift::protocol::TJSONProtocolFactory;	\
		using ::apache::thrift::protocol::TCompactProtocolFactory; \
		server_ = boost::shared_ptr<TThreadedServer>(new TThreadedServer( \
			boost::make_shared<serviceProcessorFactory>(boost::make_shared<ProcessorCloneFactory>()),	\
			boost::make_shared<TServerSocket>(port_), \
			boost::make_shared<TBufferedTransportFactory>(),	\
			boost::make_shared<protoc_fac_t>()));	\
		if (eventHandler != nullptr) {	\
			server_->setServerEventHandler(eventHandler); \
		} \
	} \
	int port_; \
	int threadCount_; \
	boost::shared_ptr<::apache::thrift::concurrency::ThreadManager> threadManager_; \
	boost::shared_ptr<::apache::thrift::server::TThreadedServer> server_;	 \
	THost(const THost &); \
	THost & operator=(const THost &); \
}; \
} \
}


#define DECLARE_THREADED_SERVER(serviceName, __protocol) THRIFT_SERVICE(XXX_IfFactory(serviceName), \
XXX_If(serviceName), \
XXX_Handler(serviceName), \
XXX_ProcessorFactory(serviceName), \
XXX_ProtocolFactory(__protocol))


#endif // __THRIFT_SERVER__