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
// 2016-05-11		    -- iegad		1, 改用范型与宏来定义服务端, 优点是, 可读性高, 方便代码调试



#include "thrift_ex_macro.h"
#include "thrift_eventhandler.hpp"
#include "tools/iegad_log.hpp"



namespace iegad { 
namespace thrift_ex { 
 


    // ============================
    // @用途 : 处理机工厂模板
    // @__SVC_IFFAC_T_ : RPC接口工厂
    // @__SVC_IF_T_ : RPC接口
    // @__SVC_HANDLER_T_ : RPC接口实现句柄类
    // ============================
    template <typename __SVC_IFFAC_T_, typename __SVC_IF_T_, typename __SVC_HANDLER_T_>
    class ProcessorCloneFactory : virtual public __SVC_IFFAC_T_ {
    // 处理机工厂模板类
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
    // @用途 : 扩展thrift服务端, 以提供更友好的使用接口
    // @__SVC_PROCESSOR_FAC_T_ : 处理机工厂接口类(由thrift生成), 
    //							名称一般是 xxxProcessorFactory;
    // @__SVC_PROCESSOR_CLONE_FAC_T_ : 处理机工厂实现类, 由自己实现, ProcessorCloneFactory<T1, T2, T3>;
    // @__SERVER_T_ : 服务端模型类, 可选模型有:
    //					1) per-connect, 
    //					2) connect-pool, 
    //					3) IO-overlapping;
    // @__PROTOCOL_FAC_T_ : 协议工厂类, 可选协议有:
    //					1) json, 
    //					2) binary, 
    //					3) compact(varint);
    // ============================
    template <typename __SVC_PROCESSOR_FAC_T_, 
    	typename __SVC_PROCESSOR_CLONE_FAC_T_,
    	typename __SERVER_T_,
    	typename __PROTOCOL_FAC_T_>
    class THost {
    // thrift服务端模板类
    public:
	// ============================
	// @用途 : 内置类型定义
	// ============================
	// 并发性工具
	typedef ::apache::thrift::concurrency::ThreadManager		ThreadManager;
	typedef ::apache::thrift::concurrency::PlatformThreadFactory	PlatformThreadFactory;
	typedef ::apache::thrift::transport::TBufferedTransportFactory	TBufferedTransportFactory;
	// 服务端
	typedef ::apache::thrift::transport::TServerSocket			TServerSocket;
	typedef ::apache::thrift::server::TServerEventHandler			TServerEventHandler;
	typedef ::apache::thrift::server::TServer					TServer;
	typedef ::apache::thrift::server::TThreadedServer			TThreadedServer;
	typedef ::apache::thrift::server::TThreadPoolServer			TThreadPoolServer;
	typedef ::apache::thrift::server::TNonblockingServer			TNonblockingServer;
	// 协议
	typedef __PROTOCOL_FAC_T_							protoc_fac_t;
	typedef ::apache::thrift::protocol::TBinaryProtocolFactory		TBinaryProtocolFactory;
	typedef ::apache::thrift::protocol::TJSONProtocolFactory		TJSONProtocolFactory;
	typedef ::apache::thrift::protocol::TCompactProtocolFactory	TCompactProtocolFactory;


	// ============================
	// @用途 : 构造函数
	// @port : 端口号
	// @eventHandler : 事件句柄
	// @threadCount : 工作线程数/ 最大客户端连接数
	// @PS : windows平台下, 当调用_socket_init方法失败时, 会抛出异常
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
	// @用途 : 析构函数
	// ============================
	~THost() {
	    _socket_release();
	    server_->stop();
	}


	// ============================
	// @用途 : 启动服务
	// @返回值 : void
	// ============================
	void Run() {
	    iINFO << "===============================\n";
	    iINFO << VERSION_TYPE;
	    iINFO << "Server running : listened at 0.0.0.0 : " << port_ << std::endl;
	    iINFO << "===============================\n\n";
	    server_->run();
	}


	// ============================
	// @用途 : 返回 服务实现 对象指针, 以方便用户对服务端进
	//            更多的配置.
	// @返回值 : 当前服务器类型的指针
	// ============================
	boost::shared_ptr<__SERVER_T_> GetServer() {
	    return boost::dynamic_pointer_cast<__SERVER_T_>(this->server_);
	}


    private:
	// ============================
	// @用途 : socket资源初始化
	// @返回值 : 初始化成功返回true, 否则返回false
	// @PS : 只在WINDOWS平台有意义
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
	// @用途 : socket资源回收
	// @返回值 : void
	// @PS : 只在WINDOWS平台有意义
	// ============================
	void _socket_release() {
#ifdef WIN32
	    WSACleanup();
#endif // WIN32
	} // void _socket_release();


	// ============================
	// @用途 : 构造 TThreadPool 服务端 对象
	// @eventHandler : 服务端事件句柄
	// @返回值 : 构造成功的 TThreadPool服务端 对象
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
	// @用途 : 构造 TThreadedServer 服务端 对象
	// @eventHandler : 服务端事件句柄
	// @返回值 : 构造成功的 TThreadedServer服务端 对象
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
	// @用途 : 构造 TNonblockingServer 服务端 对象
	// @eventHandler : 服务端事件句柄
	// @返回值 : 构造成功的 TNonblockingServer服务端 对象
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

	// 端口号
	int port_;
	// 工作线程数/ 最大客户端连接数
	int threadCount_;
	// 内置 服务端实现的对象指针
	boost::shared_ptr<TServer> server_;
	// 线程管理器
	boost::shared_ptr<::apache::thrift::concurrency::ThreadManager> threadManager_;


	// ============================
	// @用途 : 禁用
	// ============================
	THost(const THost &);
	THost & operator=(const THost &);
    }; // class THost;



}  // namespace thrift_ex;
} // namespace iegad;



// ============================
// @用途 : 处理机类宏定义
// @__service_name_ : 服务名称
// ============================
#define DEFINE_PROCESSOR_CLONE_FACTORY(__service_name_) \
typedef ::iegad::thrift_ex::ProcessorCloneFactory<XXX_IfFactory(__service_name_), \
XXX_If(__service_name_), \
XXX_Handler(__service_name_)> \
XXX_ProcessorCloneFactory(__service_name_);



// ============================
// @用途 : thrift服务类 宏定义
// @__service_name_ : 服务名称
// @__server_type_ : 服务类型; 
//				1) THRIFT_THREADED_SERVER, 
//				2) THRIFT_THREADPOOL_SERVER, 
//				3) THRIFT_NON_BLOCKING_SERVER;
// @__protocol_ :  协议类型; 
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
