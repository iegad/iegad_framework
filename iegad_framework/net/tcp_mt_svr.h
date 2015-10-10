#ifndef __TCP_MT_SVR__
#define __TCP_MT_SVR__



// ============ 说明 ============
//
// @创建日期 : 2015-10-02
// @创建人 : iegad
//
// ============================
// @用途 : 多线程并发服务器
//	1. 将底层通信, 消息接收进行
//	    封装, 达到 应用层 与 消息收发层 的分离.
//	2. 使用 追随者-领导者 模型;
// @PS : 在实现时应用层服务端进程时, 最后将该类
//		作为基类;
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================
// -- 2015-10-02	    -- iegad		--1. 为客户端 数据接收 设定超时值, 已防止注入攻击
//								--2. 接收 io 操作所产生的 err_code;
//  --2015-10-10	    -- iegad		--  修改构造函数, 去掉host 参数, 
//								    使用 0.0.0.0 的方式来绑定监听套接字


#include <boost/asio.hpp>


#include <mutex>
#include <vector>
#include <thread>
#include <functional>


#include "svc/basic_svc.h"


namespace iegad {
namespace net {


    using namespace boost::asio;


    class tcp_mt_svr {
    // 多线程并发服务器

    public:
	// ============================
	// @用途 : 线程池 类型声明
	// ============================
	typedef std::vector<std::thread> thread_pool_t;


	// ============================
	// @用途 : 线程互斥锁 类型声明
	// ============================
	typedef std::unique_lock<std::mutex> mtx_lock_t;	


	// ============================
	// @用途 : 基础服务类 智能指针 类型声明
	// ============================
	typedef iegad::net::basic_svc::basic_svc_ptr basic_svc_ptr;


	// ============================
	// @用途 : 服务对象 映射表 声明
	// ============================
	typedef iegad::net::basic_svc::svc_map_t svc_map_t;



	// ============================
	// @用途 : 构造函数
	// @host : IP地址, 不能是计算机名
	// @port : 端口号
	// ============================
	explicit tcp_mt_svr(unsigned short port);


	// ============================
	// @用途 : 析构函数
	// @PS : 因为可能被用作基类, 所以析构函数为虚函数
	// ============================
	virtual ~tcp_mt_svr();


	// ============================
	// @用途 : 开始 客户端消息接收线程
	// @n : 线程数量.
	// @返回值 : void
	// ============================
	void run(int n = 8);


	// ============================
	// @用途 : 停止 客户端消息接收线程.
	// @返回值 : void
	// ============================
	void stop();
	

	// ============================
	// @用途 : 注册 服务对象
	// @svc_obj : 服务对象, 该对象必需派生自basic_svc, 
	//		    也必需是智能指针类型
	// @返回值 : void
	// ============================
	void regist_svc(basic_svc_ptr svc_obj);

    protected:

	// ============================
	// @用途 : 服务端是否停止
	// @返回值 : 停止返回 true, 否则返回 false; 
	// ============================
	bool _is_stop();

    private:
	// ============================
	// @用途 : 客户端消息接收线程
	// @返回值 : 停止返回 true, 否则返回 false; 
	// ============================
	void _thread_proc();


	// ============================
	// @用途 : 等待客户端连接, 成功连接后, 将客户端会话保存在 clnt;
	// @clnt : 待接收的客户端对象
	// @err_code : 当发送错误时, 用来接收错误
	// @返回值 : 客户端成功建立连接返回 0, 否则返回 -1; 
	// ============================
	int _accept(ip::tcp::socket & clnt, 
	    boost::system::error_code & err_code);


	// ============================
	// @用途 : 等待客户端 clnt 发送消息, 消息接收后, 将消息写入 msgbsc;
	// @clnt : 待接收的客户端
	// @msgbsc : 用来保存客户端发送的消息.
	// @err_code : 当发送错误时, 用来接收错误
	// @返回值 : 接收到消息并成功构建 basic_msg 对象 返回 0, 否则返回 -1; 
	// ============================
	int _build_basic_msg(ip::tcp::socket & clnt, iegad::net::basic_msg & msgbsc, 
	    boost::system::error_code & err_code);


	// ============================
	// @用途 : 服务调用.
	// @clnt : 请求服务的客户端
	// @msgbsc : 用来构建 应用服务对象 的basic_msg对象.
	// @返回值 : 成功发送服务调用 返回 0, 否则返回 -1; 
	// @PS : 这里应注意, 只要 服务被调用, 就会返回0, 
	//	    返回0, 并不能说明 服务调用 是正确的结束的.
	// ============================
	int _call_svc(ip::tcp::socket & clnt, iegad::net::basic_msg & msgbsc);


	// 停止标志
	bool stop_flag_;
	// 客户端消息接收线程锁
	std::mutex thread_mtx_;
	// 停止标志锁
	std::mutex stop_mtx_;
	// 监听对象的 boost::io_service
	io_service ios_;
	// 客户端消息接收线程 线程池
	thread_pool_t thread_pool_;
	// 服务映射表
	svc_map_t svc_map_;
	// 监听对象
	ip::tcp::acceptor acptor_;
	// 超时值 
	const int timeout_ = 5000;


	// 禁用
	tcp_mt_svr(const tcp_mt_svr &);
	tcp_mt_svr & operator=(const tcp_mt_svr &);
    };

} // namespace net;
} // namespace iegad


#endif // __TCP_MT_SVR__