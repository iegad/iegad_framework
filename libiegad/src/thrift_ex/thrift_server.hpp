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
// 2016-04-19		    -- iegad		1, ȥ�����������ĸ���, ��Ϊ�ú궨��.
//								2, ���NonBlockingServer��ThreadedServer���ֲ�ͬ�ķ�����ģ��
//								3, ȥ��InitEnvironment��̬����
// 2016-05-06		    -- iegad		1, ���ú�����������



#include "thrift_ex_macro.h"
#include "thrift_eventhandler.hpp"
#include "tools/iegad_log.hpp"



// ============================
// @��; : socket��Դ��ʼ��
// @����ֵ : ��ʼ���ɹ�����true, ���򷵻�false
// @PS : ֻ��WINDOWSƽ̨��Ч
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
// @��; : socket��Դ����
// @����ֵ : void
// @PS : ֻ��WINDOWSƽ̨��Ч
// ============================
static inline void
socket_release()
{
#ifdef WIN32
	WSACleanup();
#endif // WIN32
}



// ============================
// @��; : ��������������
// @serviceIfFactory : RPC�ӿڹ���
// @serviceIf : RPC�ӿ�
// @serviceHandler : RPC�ӿ�ʵ�־����
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
// @��; : per-connect ģʽ����˶����
// @serviceName : ��������
// @serviceIfFactory : RPC�ӿڹ���
// @serviceIf : RPC�ӿ�
// @serviceHandler : RPC�ӿ�ʵ�־����
// @serviceProcessorFactory : ���������
// @protocol_type : Э������; ֧�ֵ�Э���� Json, binary, compact
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
// @��; : connect-pool ģʽ����˶����
// @serviceName : ��������
// @serviceIfFactory : RPC�ӿڹ���
// @serviceIf : RPC�ӿ�
// @serviceHandler : RPC�ӿ�ʵ�־����
// @serviceProcessorFactory : ���������
// @protocol_type : Э������; ֧�ֵ�Э���� Json, binary, compact
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
// @��; : IO����+�������׽��� ģʽ����˶����
// @serviceName : ��������
// @serviceIfFactory : RPC�ӿڹ���
// @serviceIf : RPC�ӿ�
// @serviceHandler : RPC�ӿ�ʵ�־����
// @serviceProcessorFactory : ���������
// @protocol_type : Э������; ֧�ֵ�Э���� Json, binary, compact
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
// @��; : per-connect ģʽ����˶����(�ӿں�, ���û�ʹ�õĲ���)
// @serviceName : ��������
// @__protocol : Э������; ֧�ֵ�Э���� Json, binary, compact
// ============================
#define DEFINE_THREADED_SERVER(serviceName, __protocol) THRIFT_THREADED_SERVICE(serviceName, \
XXX_IfFactory(serviceName), \
XXX_If(serviceName), \
XXX_Handler(serviceName), \
XXX_ProcessorFactory(serviceName), \
XXX_ProtocolFactory(__protocol))


// ============================
// @��; : connect-pool ģʽ����˶����(�ӿں�, ���û�ʹ�õĲ���)
// @serviceName : ��������
// @__protocol : Э������; ֧�ֵ�Э���� Json, binary, compact
// ============================
#define DEFINE_THREADPOOL_SERVER(serviceName, __protocol) THRIFT_THREAD_POOL_SERVICE(serviceName, \
XXX_IfFactory(serviceName), \
XXX_If(serviceName), \
XXX_Handler(serviceName), \
XXX_ProcessorFactory(serviceName), \
XXX_ProtocolFactory(__protocol))


// ============================
// @��; : IO����+�������׽��� ģʽ����˶����(�ӿں�, ���û�ʹ�õĲ���)
// @serviceName : ��������
// @__protocol : Э������; ֧�ֵ�Э���� Json, binary, compact
// ============================
#define DEFINE_NON_BLOCKING_SERVER(serviceName, __protocol) THRIFT_NON_BLOCKING_SERVICE(serviceName, \
XXX_IfFactory(serviceName), \
XXX_If(serviceName), \
XXX_Handler(serviceName), \
XXX_ProcessorFactory(serviceName), \
XXX_ProtocolFactory(__protocol))


#endif // __THRIFT_SERVER__