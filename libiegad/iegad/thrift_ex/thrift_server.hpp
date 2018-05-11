#ifndef __THRIFT_SERVER__
#define __THRIFT_SERVER__



// ============ 说明 ============
//
// @创建日期 : 2016-02-23
// @创建人 : iegad
//
// ============================
// @用途 : 1, 对thrift 三种Server模型的扩展
//        2, 提供快速开发
//
// @PS : 该文件依赖于 thrift 开源库    
//
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================
// 2016-03-04           -- iegad         -- 1, 去掉 构造函数中的 threadCount的默认值( default value = 4).
//                                       -- 2, 增加 ThreadServer 模型, 通过 重载构造函数的方式实现.
//                                       -- 3, 将 server_ 类型 由 ThreadPoolServer 替换为 TServerFramework, 为了实现多态.
// 2016-04-19           -- iegad         -- 1, 去掉服务器包的概念, 改为用宏定义.
//                                       -- 2, 添加NonBlockingServer和ThreadedServer两种不同的服务器模型
//                                       -- 3, 去掉InitEnvironment静态函数
// 2016-05-06           -- iegad         -- 1, 改用宏来定义服务端
// 2016-05-11           -- iegad         -- 1, 改用范型与宏来定义服务端, 优点是, 可读性高, 方便代码调试
// 2018-05-11           -- iegad         -- 1, thrift升级为0.11.0版, 替换boost::shared_ptr 为stdcxx::shared_ptr
//                                       -- 2, format codes
//                                       -- 3, 只保留nonblockingServer和Compact协议 以减小代码量



#include "iegad/thrift_ex/thrift_ex_macro.h"





namespace iegad { 
namespace thrift_ex {


using namespace apache::thrift;



// ============================
// @用途 : 处理机工厂模板
// @__SVC_IFFAC_T_ : RPC接口工厂
// @__SVC_IF_T_ : RPC接口
// @__SVC_HANDLER_T_ : RPC接口实现句柄类
// ============================
template <typename __SVC_IFFAC_T_, typename __SVC_IF_T_, typename __SVC_HANDLER_T_>
class processorCloneFactory : virtual public __SVC_IFFAC_T_ {
// 处理机工厂模板类
public:
    typedef transport::TSocket TSocket;


    virtual ~processorCloneFactory() {}


    virtual __SVC_IF_T_*
    getHandler(const TConnectionInfo & connInfo) 
    {
        auto sock = stdcxx::dynamic_pointer_cast<TSocket>(connInfo.transport);
        sock->setRecvTimeout(_APP_TIME_OUT);
        sock->setNoDelay(true);
        sock->setKeepAlive(true);

        return new __SVC_HANDLER_T_;
    }


    virtual void
    releaseHandler(__SVC_IF_T_ * handler) 
    {
        delete handler;
    }
}; // class ProcessorCloneFactory;



// ============================
// @用途 : 扩展thrift服务端, 以提供更友好的使用接口
// @SvcProcessorFactory : 处理机工厂接口类(由thrift生成),
//                          名称一般是 xxxProcessorFactory;
// @SvcProcessorCloneFactory : 处理机工厂实现类, 由自己实现, ProcessorCloneFactory<T1, T2, T3>;
// ============================
template <typename SvcProcessorFactory,
    typename SvcProcessorCloneFactory>
class thriftServer {
// thrift服务端模板类
public:
    // ============================
    // @用途 : 内置类型定义
    // ============================
    typedef concurrency::ThreadManager                   ThreadManager;
    typedef stdcxx::shared_ptr<ThreadManager>            ThreadManagerPtr;
    typedef concurrency::PlatformThreadFactory           PlatformThreadFactory;
    typedef stdcxx::shared_ptr<PlatformThreadFactory>    PlatformThreadFactoryPtr;
    typedef transport::TNonblockingServerSocket          NonblockingServerSocket;
    typedef stdcxx::shared_ptr<NonblockingServerSocket>  NonblockingServerSocketPtr;
    typedef server::TServerEventHandler                  ServerEventHandler;
    typedef stdcxx::shared_ptr<ServerEventHandler>       ServerEventHandlerPtr;
    typedef server::TServer                              Server;
    typedef stdcxx::shared_ptr<Server>                   ServerPtr;
    typedef server::TNonblockingServer                   NonblockingServer;
    typedef stdcxx::shared_ptr<NonblockingServer>        NonblockingServerPtr;
    typedef protocol::TCompactProtocolFactory            CompactProtocolFactory;
    typedef stdcxx::shared_ptr<CompactProtocolFactory>   CompactProtocolFactoryPtr;
    typedef stdcxx::shared_ptr<SvcProcessorCloneFactory> SvcProcessorCloneFactoryPtr;
    typedef stdcxx::shared_ptr<SvcProcessorFactory>      SvcProcessorFactoryPtr;
    typedef protocol::TCompactProtocolFactory            ProtocolFactory;
    typedef stdcxx::shared_ptr<ProtocolFactory>          ProtocolFactoryPtr;


    // ============================
    // @用途 : 构造函数
    // @port : 端口号
    // @eventHandler : 事件句柄
    // @threadCount : 工作线程数/ 最大客户端连接数
    // @PS : windows平台下, 当调用_socket_init方法失败时, 会抛出异常
    // ============================
    thriftServer(int port, int threadCount = 4) :
        port_(port),
        threadCount_(threadCount)
    {
        assert(threadCount > 0 && port > 0 && port <= 65535);
        if (!_socket_init()) {
            throw std::logic_error("Windows socket init failed");
        }

        server_ = _init_nonblock_svr();
    }


    // ============================
    // @用途 : 构造函数
    // @port : 端口号
    // @eventHandler : 事件句柄
    // @threadCount : 工作线程数/ 最大客户端连接数
    // @PS : windows平台下, 当调用_socket_init方法失败时, 会抛出异常
    // ============================
    thriftServer(const std::string &host, int port, int threadCount = 4) :
        port_(port),
        threadCount_(threadCount),
        host_(host)
    {
        assert(host.length() > 0 && port > 0 && port <= 65535 && threadCount > 0);

        if (!_socket_init()) {
            throw std::logic_error("Windows socket init failed");
        }

        server_ = _init_nonblock_svr();
    }


    // ============================
    // @用途 : 析构函数
    // ============================
    ~thriftServer()
    {
        _socket_release();
        server_->stop();
    }

    // ============================
    // @用途 : 启动服务
    // @返回值 : void
    // ============================
    void 
    run() 
    {
        server_->run();
    }


    // ============================
    // @用途 : 返回 服务实现 对象指针, 以方便用户对服务端进
    //            更多的配置.
    // @返回值 : 当前服务器类型的指针
    // ============================
    ServerPtr
    imp()
    {
        return server_;
    }


private:
    // ============================
    // @用途 : socket资源初始化        
    // @返回值 : 初始化成功返回true, 否则返回false
    // @PS : 只在WINDOWS平台有意义
    // ============================
    bool 
    _socket_init() 
    {
        #ifdef WIN32
        WSADATA wData;
        if (WSAStartup(0x0202, &wData) != 0) {
                return false;            }
        return HIBYTE(wData.wVersion) == 2 && LOBYTE(wData.wVersion) == 2;
        #endif // WIN32
        return true;
    }


    // ============================
    // @用途 : socket资源回收
    // @返回值 : void
    // @PS : 只在WINDOWS平台有意义        
    // ============================
    void 
    _socket_release() 
    {
        #ifdef WIN32
        WSACleanup();
        #endif // WIN32
    }

    // ============================
    // @用途 : 构造 TNonblockingServer 服务端 对象
    // @eventHandler : 服务端事件句柄
    // @返回值 : 构造成功的 TNonblockingServer服务端 对象
    // ============================
    NonblockingServerPtr
    _init_nonblock_svr() 
    {
        SvcProcessorFactoryPtr     processor(new SvcProcessorFactory(SvcProcessorCloneFactoryPtr()));
        NonblockingServerSocketPtr socket(new NonblockingServerSocket(port_));
        PlatformThreadFactoryPtr   threadFactory(new PlatformThreadFactory);
        ProtocolFactoryPtr         protocolFactory(new ProtocolFactory());

        threadManager_ = ThreadManager::newSimpleThreadManager(threadCount_);
        threadManager_->threadFactory(threadFactory);
        threadManager_->start();

        return NonblockingServerPtr(new NonblockingServer(processor, protocolFactory, socket, threadManager_));
    }


    // 端口号
    int port_;
    // 工作线程数/ 最大客户端连接数
    int threadCount_;
    // 内置 服务端实现的对象指针
    ServerPtr server_;
    // 线程管理器
    ThreadManagerPtr threadManager_;

    // 服务器ip
    std::string host_;

    // ============================
    // @用途 : 禁用
    // ============================
    thriftServer(const thriftServer &);
    thriftServer & operator=(const thriftServer &);
}; // class thriftServer;



}  // namespace thrift_ex;
} // namespace iegad;



// ============================
// @用途 : 处理机类宏定义
// @__service_name_ : 服务名称
// ============================
#define DEFINE_PROCESSOR_CLONE_FACTORY(__service_name_) \
typedef ::iegad::thrift_ex::processorCloneFactory<XXX_IfFactory(__service_name_), \
XXX_If(__service_name_), \
XXX_Handler(__service_name_)> \
XXX_ProcessorCloneFactory(__service_name_);



// ============================
// @用途 : thrift服务类 宏定义
// @__service_name_ : 服务名称
// ============================
#define DEFINE_THRIFT_SERVER(__service_name_) \
DEFINE_PROCESSOR_CLONE_FACTORY(__service_name_) \
typedef ::iegad::thrift_ex::thriftServer<XXX_ProcessorFactory(__service_name_), \
XXX_ProcessorCloneFactory(__service_name_)> \
__service_name_;



#endif // __THRIFT_SERVER__
