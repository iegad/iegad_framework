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