#ifndef __THRIFT_SERVER__
#define __THRIFT_SERVER__



// ============ ˵�� ============
//
// @�������� : 2016-02-23
// @������ : iegad
//
// ============================
// @��; : 1, ��thrift ����Serverģ�͵���չ
//				2, �ṩ���ٿ���
//		
// @PS : ���ļ������� thrift ��Դ��    
//
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================
// 2016-03-04		    -- iegad		1, ȥ�� ���캯���е� threadCount��Ĭ��ֵ( default value = 4).
//								2, ���� ThreadServer ģ��, ͨ�� ���ع��캯���ķ�ʽʵ��.
//								3, �� server_ ���� �� ThreadPoolServer �滻Ϊ TServerFramework, Ϊ��ʵ�ֶ�̬.
// 2016-04-19			-- iegad			1, ȥ�����������ĸ���, ��Ϊ�ú궨��.
//														2, ���NonBlockingServer��ThreadedServer���ֲ�ͬ�ķ�����ģ��
//														3, ȥ��InitEnvironment��̬����



#ifndef THRIFT_SERVICE_NAME
#error �붨��thrift��������
#endif


#if defined(THREADED_SERVER) && defined(NON_BLOCK_SERVER)
#error ����ͬʱ�������ַ���������
#endif



#define VERSION_TYPE "V0.1.1"	    // ��ǰ thrift_ex �汾��.



#define __XXX_IfFactory__(a)					a##IfFactory
#define _XXX_IfFactory(a)						__XXX_IfFactory__(a)
#define __XXX_If__(a)							a##If
#define _XXX_If(a)									__XXX_If__(a)
#define __XXX_Handler__(a)					a##Handler
#define _XXX_Handler(a)						__XXX_Handler__(a)
#define __XXX_ProcessorFactory__(a)	a##ProcessorFactory
#define _XXX_ProcessorFactory(a)			__XXX_ProcessorFactory__(a)	


#ifndef NON_BLOCK_SERVER
#define _APP_TIME_OUT 10  // 10�볬ʱֵ
#else
#define _APP_TIME_OUT 300 // 5���ӳ�ʱֵ
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
		// Handler����
	public:
		// ============================
		// @��; : ��������
		// ============================
		virtual ~ProcessorCloneFactory() {}


		// ============================
		// @��; : ���� XXX_Handler ����ʵ��
		// @connInfo : �ͻ���������Ϣ.
		// @����ֵ : XXX_Handler ʵ��.
		// ============================
		virtual _XXX_If(THRIFT_SERVICE_NAME)* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
		{
			// ����������鿴�����ӵ� �ͻ��� ����Ϣ
			boost::shared_ptr<::apache::thrift::transport::TSocket> sock =
				boost::dynamic_pointer_cast<::apache::thrift::transport::TSocket>(connInfo.transport);
			std::cout << "Client connected : " << sock->getOrigin() << '\n' << std::endl;
			return new _XXX_Handler(THRIFT_SERVICE_NAME);
		}


		// ============================
		// @��; : �ͷ� handler ����.
		// @handler : ��Ҫ�ͷŵ���Դ����.
		// @����ֵ : void
		// ============================
		virtual void releaseHandler(_XXX_If(THRIFT_SERVICE_NAME) * handler) {
			delete handler;
		}
	}; // class BasicCloneFactory;



	class THost {
		// ���̳߳� Server��
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
		// @��; : ���캯��.
		// @port : �˿ں�.
		// @threadCount : �����߳���
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
		// @��; : ���������� Server.
		// @����ֵ : void
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
		// @��; : ���ó�ʼ��
		// @eventHandler : �¼����
		// @����ֵ : void
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
			 *	widnows �³�ʼ��winsock2����
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


		// �˿�
		int port_;
		// �����߳���
		int threadCount_;
		// �̵߳�����
		boost::shared_ptr<::apache::thrift::concurrency::ThreadManager> threadManager_;
		// Server ����ʵ����ָ��
#if defined(NON_BLOCK_SERVER)
		boost::shared_ptr<::apache::thrift::server::TNonblockingServer> server_;
#elif defined(THREADED_SERVER)
		boost::shared_ptr<::apache::thrift::server::TThreadedServer> server_;
#else
		boost::shared_ptr<::apache::thrift::server::TThreadPoolServer> server_;
#endif

		// ============================
		// @��; : ��ֹ ���� ��ֵ����
		// ============================
		THost(const THost &);
		THost & operator=(const THost &);
	}; // class THost;



} // namespace thrift_ex;
} // namespace iegad;



#endif // __THRIFT_SERVER__