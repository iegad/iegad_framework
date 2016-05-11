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
// 2016-05-11		    -- iegad		1, ���÷����������������, �ŵ���, �ɶ��Ը�, ����������



#include "thrift_ex_macro.h"
#include "thrift_eventhandler.hpp"
#include "tools/iegad_log.hpp"



namespace iegad { 
namespace thrift_ex { 
 

    // ============================
    // @��; : ���������ģ��
    // @__SVC_IFFAC_T_ : RPC�ӿڹ���
    // @__SVC_IF_T_ : RPC�ӿ�
    // @__SVC_HANDLER_T_ : RPC�ӿ�ʵ�־����
    // ============================
    template <typename __SVC_IFFAC_T_, typename __SVC_IF_T_, typename __SVC_HANDLER_T_>
    class ProcessorCloneFactory : virtual public __SVC_IFFAC_T_ {
    public:
	virtual ~ProcessorCloneFactory() {}

	virtual __SVC_IF_T_ * getHandler(const ::apache::thrift::TConnectionInfo & connInfo) {
	    boost::shared_ptr<::apache::thrift::transport::TSocket> sock =
		boost::dynamic_pointer_cast<::apache::thrift::transport::TSocket>(connInfo.transport);
	    sock->setRecvTimeout(_APP_TIME_OUT);
	    return new __SVC_HANDLER_T_;
	} // virtual __SVC_IF_T_ * getHandler(const ::apache::thrift::TConnectionInfo &);

	virtual void releaseHandler(__SVC_IF_T_ * handler) {
	    delete handler;
	} // virtual void releaseHandler(__SVC_IF_T_ *);
    }; // class ProcessorCloneFactory;


    // ============================
    // @��; : ��չthrift�����, ���ṩ���Ѻõ�ʹ�ýӿ�
    // @__SVC_PROCESSOR_FAC_T_ : ����������ӿ���(��thrift����), 
    //							����һ���� xxxProcessorFactory;
    // @__SVC_PROCESSOR_CLONE_FAC_T_ : ���������ʵ����, ���Լ�ʵ��, ProcessorCloneFactory<T1, T2, T3>;
    // @__SERVER_T_ : �����ģ����, ��ѡģ����:
    //					1) per-connect, 
    //					2) connect-pool, 
    //					3) IO-overlapping;
    // @__PROTOCOL_FAC_T_ : Э�鹤����, ��ѡЭ����:
    //					1) json, 
    //					2) binary, 
    //					3) compact(varint);
    // ============================
    template <typename __SVC_PROCESSOR_FAC_T_, 
    	typename __SVC_PROCESSOR_CLONE_FAC_T_,
    	typename __SERVER_T_,
    	typename __PROTOCOL_FAC_T_>
    class THost {
    public:
	// ============================
	// @��; : �������Ͷ���
	// ============================
	// �����Թ���
	typedef ::apache::thrift::concurrency::ThreadManager		ThreadManager;
	typedef ::apache::thrift::concurrency::PlatformThreadFactory	PlatformThreadFactory;
	typedef ::apache::thrift::transport::TBufferedTransportFactory	TBufferedTransportFactory;
	// �����
	typedef ::apache::thrift::transport::TServerSocket			TServerSocket;
	typedef ::apache::thrift::server::TServerEventHandler			TServerEventHandler;
	typedef ::apache::thrift::server::TServer					TServer;
	typedef ::apache::thrift::server::TThreadedServer			TThreadedServer;
	typedef ::apache::thrift::server::TThreadPoolServer			TThreadPoolServer;
	typedef ::apache::thrift::server::TNonblockingServer			TNonblockingServer;
	// Э��
	typedef __PROTOCOL_FAC_T_							protoc_fac_t;
	typedef ::apache::thrift::protocol::TBinaryProtocolFactory		TBinaryProtocolFactory;
	typedef ::apache::thrift::protocol::TJSONProtocolFactory		TJSONProtocolFactory;
	typedef ::apache::thrift::protocol::TCompactProtocolFactory	TCompactProtocolFactory;


	// ============================
	// @��; : ���캯��
	// @port : �˿ں�
	// @eventHandler : �¼����
	// @threadCount : �����߳���/ ���ͻ���������
	// @PS : windowsƽ̨��, ������_socket_init����ʧ��ʱ, ���׳��쳣
	// ============================
	explicit THost(int port,
	    boost::shared_ptr<TServerEventHandler> eventHandler = nullptr,
	    int threadCount = 4)
	    :
	    threadCount_(threadCount),
	    port_(port),
	    server_(nullptr) {
	    if (!_socket_init()) {
		throw std::exception("Windows socket init failed");
	    }
	    if (typeid(__SERVER_T_) == typeid(TThreadedServer)) {
		server_ = _init_threaded_svr(eventHandler);
	    }
	    else if (typeid(__SERVER_T_) == typeid(TThreadPoolServer)) {
		server_ = _init_threadpool_svr(eventHandler);
	    }
	    else if (typeid(__SERVER_T_) == typeid(TNonblockingServer)) {
		server_ = _init_nonblock_svr(eventHandler);
	    }
	}


	// ============================
	// @��; : ��������
	// ============================
	~THost() {
	    _socket_release();
	    server_->stop();
	}


	// ============================
	// @��; : ��������
	// @����ֵ : void
	// ============================
	void Run() {
	    iINFO << "===============================\n";
	    iINFO << VERSION_TYPE;
	    iINFO << "Server running : listened at 0.0.0.0 : " << port_ << std::endl;
	    iINFO << "===============================\n\n";
	    server_->run();
	}


	// ============================
	// @��; : ���� ����ʵ�� ����ָ��, �Է����û��Է���˽�
	//            ���������.
	// @����ֵ : ��ǰ���������͵�ָ��
	// ============================
	boost::shared_ptr<__SERVER_T_> GetServer() {
	    return boost::dynamic_pointer_cast<__SERVER_T_>(this->server_);
	}


    private:
	// ============================
	// @��; : socket��Դ��ʼ��
	// @����ֵ : ��ʼ���ɹ�����true, ���򷵻�false
	// @PS : ֻ��WINDOWSƽ̨������
	// ============================
	bool _socket_init() {
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
	// @PS : ֻ��WINDOWSƽ̨������
	// ============================
	void _socket_release() {
#ifdef WIN32
	    WSACleanup();
#endif // WIN32
	} // void _socket_release();


	// ============================
	// @��; : ���� TThreadPool ����� ����
	// @eventHandler : ������¼����
	// @����ֵ : ����ɹ��� TThreadPool����� ����
	// ============================
	boost::shared_ptr<TThreadPoolServer> _init_threadpool_svr(boost::shared_ptr<TServerEventHandler> eventHandler) {
	    threadManager_ = ThreadManager::newSimpleThreadManager(threadCount_);
	    threadManager_->threadFactory(boost::shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory()));
	    threadManager_->start();
	    boost::shared_ptr<TThreadPoolServer> serv(new TThreadPoolServer(
		boost::make_shared<__SVC_PROCESSOR_FAC_T_>(boost::make_shared<__SVC_PROCESSOR_CLONE_FAC_T_>()),
		boost::make_shared<TServerSocket>(port_),
		boost::make_shared<TBufferedTransportFactory>(),
		boost::make_shared<protoc_fac_t>(),
		threadManager_));
	    if (eventHandler != nullptr) {
		serv->setServerEventHandler(eventHandler);
	    }
	    return serv;
	}


	// ============================
	// @��; : ���� TThreadedServer ����� ����
	// @eventHandler : ������¼����
	// @����ֵ : ����ɹ��� TThreadedServer����� ����
	// ============================
	boost::shared_ptr<TThreadedServer> _init_threaded_svr(boost::shared_ptr<TServerEventHandler> eventHandler) {
	    boost::shared_ptr<TThreadedServer> serv(new TThreadedServer(
		boost::make_shared<__SVC_PROCESSOR_FAC_T_>(boost::make_shared<__SVC_PROCESSOR_CLONE_FAC_T_>()),
		boost::make_shared<TServerSocket>(port_),
		boost::make_shared<TBufferedTransportFactory>(),
		boost::make_shared<protoc_fac_t>()));
	    serv->setConcurrentClientLimit(threadCount_);
	    if (eventHandler != nullptr) {
		serv->setServerEventHandler(eventHandler);
	    }
	    return serv;
	}


	// ============================
	// @��; : ���� TNonblockingServer ����� ����
	// @eventHandler : ������¼����
	// @����ֵ : ����ɹ��� TNonblockingServer����� ����
	// ============================
	boost::shared_ptr<TNonblockingServer> _init_nonblock_svr(boost::shared_ptr<TServerEventHandler> eventHandler) {
	    threadManager_ = ThreadManager::newSimpleThreadManager(threadCount_);
	    threadManager_->threadFactory(boost::shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory()));
	    threadManager_->start();
	    boost::shared_ptr<TNonblockingServer> serv(new TNonblockingServer(
		boost::make_shared<__SVC_PROCESSOR_FAC_T_>(boost::make_shared<__SVC_PROCESSOR_CLONE_FAC_T_>()),
		boost::make_shared<TBufferedTransportFactory>(),
		boost::make_shared<TBufferedTransportFactory>(),
		boost::make_shared<protoc_fac_t>(),
		boost::make_shared<protoc_fac_t>(),
		port_,
		threadManager_));
	    serv->setMaxConnections(threadCount_);
	    if (eventHandler != nullptr) {
		serv->setServerEventHandler(eventHandler);
	    }
	    return serv;
	}

	// �˿ں�
	int port_;
	// �����߳���/ ���ͻ���������
	int threadCount_;
	// ���� �����ʵ�ֵĶ���ָ��
	boost::shared_ptr<TServer> server_;
	// �̹߳�����
	boost::shared_ptr<::apache::thrift::concurrency::ThreadManager> threadManager_;


	// ============================
	// @��; : ����
	// ============================
	THost(const THost &);
	THost & operator=(const THost &);
    }; // class THost;


}  // namespace thrift_ex;
} // namespace iegad;



// ============================
// @��; : �������궨��
// @__service_name_ : ��������
// ============================
#define DEFINE_PROCESSOR_CLONE_FACTORY(__service_name_) \
typedef ::iegad::thrift_ex::ProcessorCloneFactory<XXX_IfFactory(__service_name_), \
XXX_If(__service_name_), \
XXX_Handler(__service_name_)> \
XXX_ProcessorCloneFactory(__service_name_);



// ============================
// @��; : thrift������ �궨��
// @__service_name_ : ��������
// @__server_type_ : ��������; 
//				1) THRIFT_THREADED_SERVER, 
//				2) THRIFT_THREADPOOL_SERVER, 
//				3) THRIFT_NON_BLOCKING_SERVER;
// @__protocol_ :  Э������; 
//				1) BINARY_PROTOCOL, 
//				2) COMPACT_PROTOCOL, 
//				3) JSON_PROTOCOL;
// ============================
#define DEFINE_THRIFT_SERVER(__service_name_, __server_type_, __protocol_) \
DEFINE_PROCESSOR_CLONE_FACTORY(__service_name_) \
typedef ::iegad::thrift_ex::THost<XXX_ProcessorFactory(__service_name_), \
XXX_ProcessorCloneFactory(__service_name_), \
__server_type_, \
XXX_ProtocolFactory(__protocol_)> \
__service_name_;



#endif // __THRIFT_SERVER__