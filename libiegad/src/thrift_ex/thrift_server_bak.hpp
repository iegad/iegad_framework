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
// 2016-04-19			-- iegad			1, 去掉服务器包的概念, 改为用宏定义.
//														2, 添加NonBlockingServer和ThreadedServer两种不同的服务器模型
//														3, 去掉InitEnvironment静态函数



#ifndef THRIFT_SERVICE_NAME
#error 请定义thrift服务名称
#endif


#if defined(THREADED_SERVER) && defined(NON_BLOCK_SERVER)
#error 不能同时定义两种服务器类型
#endif



#define VERSION_TYPE "V0.1.1"	    // 当前 thrift_ex 版本号.



#define __XXX_IfFactory__(a)					a##IfFactory
#define _XXX_IfFactory(a)						__XXX_IfFactory__(a)
#define __XXX_If__(a)							a##If
#define _XXX_If(a)									__XXX_If__(a)
#define __XXX_Handler__(a)					a##Handler
#define _XXX_Handler(a)						__XXX_Handler__(a)
#define __XXX_ProcessorFactory__(a)	a##ProcessorFactory
#define _XXX_ProcessorFactory(a)			__XXX_ProcessorFactory__(a)	


#ifndef NON_BLOCK_SERVER
#define _APP_TIME_OUT 10  // 10秒超时值
#else
#define _APP_TIME_OUT 300 // 5分钟超时值
#endif



#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TSSLServerSocket.h>
#include <thrift/transport/TSSLSocket.h>
// server include
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/server/TNonblockingServer.h>
// protocol include
#include <thrift/protocol/TJSONProtocol.h>
#include <thrift/protocol/TCompactProtocol.h>


#include <boost/make_shared.hpp>


#include <iostream>
#include <ctime>


#include "string/iegad_string.h"


namespace iegad {
namespace thrift_ex {



	 


	class ProcessorCloneFactory : virtual public _XXX_IfFactory(THRIFT_SERVICE_NAME) {
		// Handler工厂
	public:
		// ============================
		// @用途 : 析构函数
		// ============================
		virtual ~ProcessorCloneFactory() {}


		// ============================
		// @用途 : 构建 XXX_Handler 对象实例
		// @connInfo : 客户端连接信息.
		// @返回值 : XXX_Handler 实例.
		// ============================
		virtual _XXX_If(THRIFT_SERVICE_NAME)* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
		{
			// 可以在这里查看所连接的 客户端 的信息
			boost::shared_ptr<::apache::thrift::transport::TSocket> sock =
				boost::dynamic_pointer_cast<::apache::thrift::transport::TSocket>(connInfo.transport);
			std::cout << "Client connected : " << sock->getOrigin() << '\n' << std::endl;
			return new _XXX_Handler(THRIFT_SERVICE_NAME);
		}


		// ============================
		// @用途 : 释放 handler 对象.
		// @handler : 需要释放的资源对象.
		// @返回值 : void
		// ============================
		virtual void releaseHandler(_XXX_If(THRIFT_SERVICE_NAME) * handler) {
			delete handler;
		}
	}; // class BasicCloneFactory;



	class THost {
		// 多线程池 Server端
	public:
#if defined(JSON_PROTOCOL)
		typedef ::apache::thrift::protocol::TJSONProtocolFactory protoc_fac_t;
#elif defined(COMPCAT_PROTOCOL)
		typedef ::apache::thrift::protocol::TCompactProtocolFactory protoc_fac_t;
#else 
		typedef ::apache::thrift::protocol::TBinaryProtocolFactory protoc_fac_t;
#endif 
		typedef ::apache::thrift::server::TServerEventHandler TServerEventHandler;


		// ============================
		// @用途 : 构造函数.
		// @port : 端口号.
		// @threadCount : 工作线程数
		// ============================
		explicit THost(int port, boost::shared_ptr<TServerEventHandler> eventHandler = nullptr, int threadCount = 4)
			:
			threadCount_(threadCount),
			port_(port),
			server_(nullptr) {
			this->_init(eventHandler);
		}


		~THost() {
#ifdef WIN32
			WSACleanup();
#endif // WIN32
			server_->stop();
		}


		// ============================
		// @用途 : 启动并运行 Server.
		// @返回值 : void
		// ============================
		void Run() {
			std::cout << "===============================\n";
			std::cout << VERSION_TYPE << std::endl;
			std::cout << "Server running...\nlistened at 0.0.0.0 : " << port_ << std::endl;
			std::cout << "===============================\n\n";
			server_->run();
		}


#if defined(NON_BLOCK_SERVER)
		boost::shared_ptr<TNonblockingServer> GetServer() {
			return this->server_;
		}
#elif defined(THREADED_SERVER)
		boost::shared_ptr<TThreadedServer> GetServer() {
			return this->server_;
		}
#else
		boost::shared_ptr<TThreadPoolServer> GetServer() {
			return this->server_;
		}
#endif


	private:
		// ============================
		// @用途 : 内置初始化
		// @eventHandler : 事件句柄
		// @返回值 : void
		// ============================
		void _init(boost::shared_ptr<TServerEventHandler> eventHandler) {
			using ::apache::thrift::concurrency::ThreadManager;
			using ::apache::thrift::concurrency::PlatformThreadFactory;
			using ::apache::thrift::transport::TServerSocket;
			using ::apache::thrift::transport::TBufferedTransportFactory;
			using ::apache::thrift::transport::TSSLSocket;
			using ::apache::thrift::transport::TSSLSocketFactory;
			using ::apache::thrift::transport::TSSLServerSocket;
			using ::apache::thrift::server::TThreadPoolServer;
			using ::apache::thrift::server::TThreadedServer;
			using ::apache::thrift::server::TNonblockingServer;
			using ::apache::thrift::protocol::TBinaryProtocolFactory;
			using ::apache::thrift::protocol::TJSONProtocolFactory;
			using ::apache::thrift::protocol::TCompactProtocolFactory;

			/* 
			 *	widnows 下初始化winsock2环境
			 */
#if defined(WIN32 ) && defined(NON_BLOCK_SERVER)
			WSAData wData;
			WSAStartup(MAKEWORD(2, 2), &wData);
			if (HIBYTE(wData.wVersion) != 2 || LOBYTE(wData.wVersion) != 2) {
				throw std::exception("WSAStartup failed");
			}
#endif // defined(WIN32 ) && defined(NON_BLOCK_SERVER)


#ifndef THREADED_SERVER
			threadManager_ = ThreadManager::newSimpleThreadManager(threadCount_);
			threadManager_->threadFactory(
				boost::shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory()));
			threadManager_->start();
#endif // THREADED_SERVER


#if defined(NON_BLOCK_SERVER)
			server_ = boost::shared_ptr<TNonblockingServer>(new TNonblockingServer(
				boost::make_shared<_XXX_ProcessorFactory(THRIFT_SERVICE_NAME)>(boost::make_shared<ProcessorCloneFactory>()),
				boost::make_shared<TBufferedTransportFactory>(),
				boost::make_shared<TBufferedTransportFactory>(),
				boost::make_shared<protoc_fac_t>(),
				boost::make_shared<protoc_fac_t>(),
				port_,
				threadManager_));
#elif defined(THREADED_SERVER)
			server_ = boost::shared_ptr<TThreadedServer>(new TThreadedServer(
				boost::make_shared<_XXX_ProcessorFactory(THRIFT_SERVICE_NAME)>(boost::make_shared<ProcessorCloneFactory>()),
				boost::make_shared<TServerSocket>(port_),
				boost::make_shared<TBufferedTransportFactory>(),
				boost::make_shared<protoc_fac_t>()));
#elif defined(OPENSSL_SERVER)
			boost::shared_ptr<TSSLSocketFactory> factory(new TSSLSocketFactory());
			factory->server(true);
			factory->authenticate(false);
			factory->loadCertificate("certificate/server.crt");
			factory->loadPrivateKey("certificate/server.key");

			boost::shared_ptr<TServerTransport> sock(new TSSLServerSocket(port_, factory));
#else
			server_ = boost::shared_ptr<TThreadPoolServer>(new TThreadPoolServer(
				boost::make_shared<_XXX_ProcessorFactory(THRIFT_SERVICE_NAME)>(boost::make_shared<ProcessorCloneFactory>()),
				boost::make_shared<TServerSocket>(port_),
				boost::make_shared<TBufferedTransportFactory>(),
				boost::make_shared<protoc_fac_t>(),
				threadManager_));
#endif


			if (eventHandler != nullptr) {
				server_->setServerEventHandler(eventHandler);
			}
		}


		// 端口
		int port_;
		// 工作线程数
		int threadCount_;
		// 线程调度器
		boost::shared_ptr<::apache::thrift::concurrency::ThreadManager> threadManager_;
		// Server 对象实例的指针
#if defined(NON_BLOCK_SERVER)
		boost::shared_ptr<::apache::thrift::server::TNonblockingServer> server_;
#elif defined(THREADED_SERVER)
		boost::shared_ptr<::apache::thrift::server::TThreadedServer> server_;
#else
		boost::shared_ptr<::apache::thrift::server::TThreadPoolServer> server_;
#endif

		// ============================
		// @用途 : 禁止 进行 赋值操作
		// ============================
		THost(const THost &);
		THost & operator=(const THost &);
	}; // class THost;



} // namespace thrift_ex;
} // namespace iegad;



#endif // __THRIFT_SERVER__