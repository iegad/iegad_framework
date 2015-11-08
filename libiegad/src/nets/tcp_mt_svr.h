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
//  --2015-10-10	    -- iegad		--  添加 tcp_mt_svr (const std::string &, const std::string &)
//								    构造函数, 以提供 计算机名 和服务名 方式绑定
//  --2015-11-08	    -- iegad		--1, 去掉对 protocbuff 的依赖.
//								    2, 将 _call_svc 改为 _action 并设定为 纯虚函数.
//								    3, 将 service 调用与 server本身分离, 去掉 regist_svc
//								    4, 超时时间改为 10 秒
//								    5, 将 msg 的 收/发函数 替换为新版本


#include <boost/asio.hpp>


#include <mutex>
#include <vector>
#include <thread>


namespace iegad {
namespace nets {


    class tcp_mt_svr {
    // 多线程并发服务器

	enum {
	// 监听队列 大小 
	    LISTENQ = 16
	};

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
	// @用途 : 构造函数
	// @port : 端口号
	// ============================
	explicit tcp_mt_svr(unsigned short port);


	// ============================
	// @用途 : 构造函数
	// @host : 机器名或IP地址
	// @svc : 端口号或服务名
	// ============================
	tcp_mt_svr(const std::string & host, const std::string & svc);


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
	// @用途 : 返回监听套接字, 所绑定的地址
	// @返回值 : 服务端 IP 地址
	// ============================
	const std::string host_endpoint();


    protected:
	// ============================
	// @用途 : 服务端是否停止
	// @返回值 : 停止返回 true, 否则返回 false; 
	// ============================
	bool _is_stop();


	// ============================
	// @用途 : 服务调用.
	// @clnt : 客户端
	// @recvbuff : 客户端缓冲区
	// @返回值 : 成功发送服务调用 返回 0, 否则返回 -1; 
	// @PS : 纯虚函数, 由派生类重写
	// ============================
	virtual int _action(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff) = 0;


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
	int _accept(boost::asio::ip::tcp::socket & clnt, 
	    boost::system::error_code & err_code);


	// 停止标志
	bool stop_flag_;
	// 客户端消息接收线程锁
	std::mutex thread_mtx_;
	// 停止标志锁
	std::mutex stop_mtx_;
	// 监听对象的 boost::io_service
	boost::asio::io_service ios_;
	// 客户端消息接收线程 线程池
	thread_pool_t thread_pool_;
	// 监听对象
	boost::asio::ip::tcp::acceptor acptor_;
	// 超时值 
#ifdef WIN32 // for win
	const int timeout_ = 10000;
#else // for unix
    const timeval timeout_ = {10, 0};
#endif // WIN32


	// 禁用
	tcp_mt_svr(const tcp_mt_svr &);
	tcp_mt_svr & operator=(const tcp_mt_svr &);
    };

} // namespace nets;
} // namespace iegad


#endif // __TCP_MT_SVR__
