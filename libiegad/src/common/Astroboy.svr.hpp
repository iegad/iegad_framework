#ifndef __ASTROBOY_SVR__
#define __ASTROBOY_SVR__



// ============ 说明 ============
//
// @创建日期 : 2016-02-23
// @创建人 : iegad
//
// ============================
// @用途 : 1, 对thrift ThreadPoolServer的扩展

// @ Astroboy_svr : 服务端进程,
//			    使用 ThreadPoolServer 作为实现.
//			    传输协议使用 BinaryProtocol .
//		
// @PS : 该文件依赖于 thrift 开源库    
//
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================


#define VERSION_TYPE "V0.1.0"	    // 当前 thrift_ex 版本号.



#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TSocket.h>

#include "thrift_clone_factory.hpp"

#include <boost/make_shared.hpp>

#include <iostream>
#include <functional>


namespace iegad {
namespace thrift_ex {


    class Astroboy_svr {
    // Astroboy 多线程池 Server端
    public:
	typedef std::function<bool(void)> fun_t;
	typedef AstroboyHandler::action_map_t action_map_t;

	// ============================
	// @用途 : 初始化环境.
	// @func : 由调用者提供环境初始化回调函数
	// @返回值 : 初始化成功, 返回 true, 否则返回 false.
	// ============================
	static bool InitEnvironment(fun_t func) {
	    return func == nullptr ? false : func();
	}


	// ============================
	// @用途 : 构造函数.
	// @port : 端口号.
	// @threadCount : 工作线程数
	// ============================
	explicit Astroboy_svr(int port, int threadCount = 4)
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
	void _init();


	// 端口
	int port_;
	// 工作线程数
	int threadCount_;
	// 线程调度器
	boost::shared_ptr<::apache::thrift::concurrency::ThreadManager> threadManager_;
	// TThreadPoolServer 对象实例的指针
	boost::shared_ptr<::apache::thrift::server::TThreadPoolServer> server_;


	// ============================
	// @用途 : 禁止 进行 赋值操作
	// ============================
	Astroboy_svr(const Astroboy_svr &);
	Astroboy_svr &  operator=(const Astroboy_svr &);
    }; // class ServerHost;


} // namespace thrift_ex;
} // namespace iegad;



#endif // __ASTROBOY_SVR__