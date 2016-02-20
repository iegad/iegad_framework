#ifndef __THRIFT_EX__
#define __THRIFT_EX__


// ============ 说明 ============
//
// @创建日期 : 2016-02-20
// @创建人 : iegad
//
// ============================
// @用途 : 1, 对thrift ThreadPoolServer的扩展
//		方便使用.
//		2, 对thirft 所生成的结构提供序列化的类.
//		
//		@PS : 该文件依赖于 thrift 开源库    
//		
//		@ Serializer : 序列化器, 
//			    用于序列化与反序列化, 提供 二进制 与 JSON串的序列化方式.
//
//		@ BasicCloneFactory : Handler工厂, 
//			    用于XXX_Handler 对象实例的 生产与销毁.
//			    该类继承自由 thrift 生成的 XXX_If 接口.
//
//		@ ServerHost : 服务端进程, 
//			    使用 ThreadPoolServer 作为实现.
//			    传输协议使用 BinaryProtocol .
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



// ============ 声明 ============
//  在包含该头文件时, 必需声明 
//	XXX_IfFactory, XXX_Handler, XXX_If, XXX_ProcessorFactory 
//  以上这些宏.
//  宏值 用thrift 所生成的对应名称, 替换 XXX_ 部分.
// ===========================
#if defined(XXX_IfFactory) && defined(XXX_Handler) && defined(XXX_If) && defined(XXX_ProcessorFactory)


#define VERSION_TYPE "V0.0.1"	    // 当前 thrift_ex 版本号.


#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/server/TThreadPoolServer.h>


#include <boost/make_shared.hpp>
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>


#include <iostream>


using namespace apache::thrift::concurrency;


template <class T>
class Serializer {
// thrift 序列化器.
public:
    // ============================
    // @用途 : 防止 实例化该类
    // ============================
    virtual ~Serializer() = 0;

    // ============================
    // @用途 : 将 T 序列化为 字节 数据
    // @data : 由thrift 生成的 struct 对象
    // @serstr : 序列化后的数据 : out参数
    // @size : 序列化后的数据大小 : out参数
    // @返回值 : 成功返回 true, 否则返回 false.
    // ============================
    static bool SerializeToArray(T & data, std::string * serstr, int * size) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TBinaryProtocol> proto(new TBinaryProtocol(buff));
	    data.write(proto.get());
	    uint8_t * p;
	    buff->getBuffer(&p, (uint32_t *)size);
	    *serstr = std::string((char *)p, *size);
	    return true;
	}
	catch (std::exception &) {
	    //...
	}
	return false;
    }


    // ============================
    // @用途 : 将 字节数据 反序列化为 T 数据对象
    // @data : 由thrift 生成的 struct 对象指针 : out参数
    // @serstr : 用于反序列化的 字节数据
    // @返回值 : 成功返回 true, 否则返回 false.
    // ============================
    static bool ParserFromArray(T * data, const std::string & serstr) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TProtocol> proto(new TBinaryProtocol(buff));
	    uint8_t * p = (uint8_t *)const_cast<char *>(serstr.c_str());
	    buff->resetBuffer(p, serstr.size());
	    data->read(proto.get());
	    return true;
	}
	catch (std::exception &) {
	
	}
	return false;
    }


    // ============================
    // @用途 : 将 T 序列化为 JSON 串
    // @data : 由thrift 生成的 struct 对象
    // @serstr : 序列化后的 JSON串 : out参数
    // @size : 序列化后的 JSON串长度 : out参数
    // @返回值 : 成功返回 true, 否则返回 false.
    // ============================
    static bool SerializeToJSONString(T & data, std::string * serstr, int * size) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TJSONProtocol> proto(new TJSONProtocol(buff));
	    data.write(proto.get());
	    uint8_t * p;
	    buff->getBuffer(&p, (uint32_t *)size);
	    *serstr = std::string((char *)p, *size);
	    return true;
	}
	catch (std::exception &) {
	    //...
	}
	return false;
    }


    // ============================
    // @用途 : 将 JSON串 反序列化为 T 数据对象
    // @data : 由thrift 生成的 struct 对象指针 : out参数
    // @serstr : 用于反序列化的 JSON串
    // @返回值 : 成功返回 true, 否则返回 false.
    // ============================
    static bool ParserFromJSONString(T * data, const std::string & serstr) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TProtocol> proto(new TJSONProtocol(buff));
	    uint8_t * p = (uint8_t *)const_cast<char *>(serstr.c_str());
	    buff->resetBuffer(p, serstr.size());
	    data->read(proto.get());
	    return true;
	}
	catch (std::exception &) {

	}
	return false;
    }
}; // class Serializer;


class BasicCloneFactory : virtual public XXX_IfFactory {
// Handler工厂
public:
    // ============================
    // @用途 : 析构函数
    // ============================
    virtual ~BasicCloneFactory() {}


    // ============================
    // @用途 : 构建 XXX_Handler 对象实例
    // @connInfo : 客户端连接信息.
    // @返回值 : XXX_Handler 实例.
    // ============================
    virtual XXX_If* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
	// 可以在这里查看所连接的 客户端 的信息
	boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
	std::cout << "Client connected : " << sock->getOrigin() << '\n' << std::endl;
	return new XXX_Handler;
    }


    // ============================
    // @用途 : 释放 handler 对象.
    // @handler : 需要释放的资源对象.
    // @返回值 : void
    // ============================
    virtual void releaseHandler(XXX_If * handler) {
	delete handler;
    }
}; // class BasicCloneFactory;


class ServerHost {
// 多线程池 Server端
public:
    // ============================
    // @用途 : 初始化环境.
    // @返回值 : 初始化成功, 返回 true, 否则返回 false.
    // @PS : XXX_Handler 必需提供InitEvironment 函数, 
    //		   该函数不会由thrift 生成, 必需手动修改.
    //		   该函数必需在构建ServerHost 实例之前被调用.
    // ============================
    static bool InitEvironment() {
	return XXX_Handler::InitEvironment();
    }


    // ============================
    // @用途 : 构造函数.
    // @port : 端口号.
    // @threadCount : 工作线程数
    // ============================
    explicit ServerHost(int port, int threadCount = 4)
	:
	threadCount_(threadCount),
	port_(port),
	server_(nullptr) {
	this->_init();
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

private:
    // ============================
    // @用途 : 内置初始化
    // @返回值 : void
    // ============================
    void _init() {

	threadManager_ = ThreadManager::newSimpleThreadManager(threadCount_);
	threadManager_->threadFactory(
	    shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory()));
	threadManager_->start();

	server_ = shared_ptr<TThreadPoolServer>(new TThreadPoolServer(
	    boost::make_shared<XXX_ProcessorFactory>(
	    boost::make_shared<BasicCloneFactory>()),
	    boost::make_shared<TServerSocket>(port_),
	    boost::make_shared<TBufferedTransportFactory>(),
	    boost::make_shared<TBinaryProtocolFactory>(),
	    threadManager_));
    }


    // 端口
    int port_;
    // 工作线程数
    int threadCount_;
    // 线程调度器
    shared_ptr<ThreadManager> threadManager_;
    // TThreadPoolServer 对象实例的指针
    shared_ptr<TThreadPoolServer> server_;


    // ============================
    // @用途 : 禁止 进行 赋值操作
    // ============================
    ServerHost(const ServerHost &);
    ServerHost &  operator=(const ServerHost &);
}; // class ServerHost;


#endif // #if defined(XXX_IfFactory) && defined(XXX_Handler) && ...
#endif // !__THRIFT_EX__
