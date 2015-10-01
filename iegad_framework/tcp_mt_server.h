#ifndef __TCP_MT_SERVER__
#define __TCP_MT_SERVER__


#include <boost/asio.hpp>


#include <mutex>
#include <vector>
#include <thread>
#include <functional>


#include "service_basic.h"


namespace iegad {
namespace net {


    using namespace boost::asio;


    class tcp_mt_svr {
    // 多线程并发服务
    enum { 
	BUF_SIZE = (1024 * 16) 
    };

    public:
	typedef std::vector<std::thread> thread_pool_t;
	typedef std::unique_lock<std::mutex> mtx_lock_t;	
	typedef iegad::net::svc_basic::svc_basic_ptr svc_basic_ptr;
	typedef iegad::net::svc_basic::svc_map_t svc_map_t;

	explicit tcp_mt_svr(
	    const std::string & host,
	    unsigned short port);

	~tcp_mt_svr();

	void run(int n = 8);

	void stop();
	
	void regist_svc(svc_basic_ptr svc_obj);

    protected:
	// check the server is running?
	bool get_stop();

    private:
	bool stop_flag_;
	std::mutex thread_mtx_;
	std::mutex stop_mtx_;
	io_service ios_;
	thread_pool_t thread_pool_;
	svc_map_t svc_map_;
	ip::tcp::acceptor acptor_;

	// for working func;
	void _thread_proc();
	// accept client connection;
	int _accept(ip::tcp::socket & clnt, boost::system::error_code & err_code);
	// build string of the "class msg_basic" from client's buffer;
	int _get_msg_basic(ip::tcp::socket & clnt, iegad::net::msg_basic & msgbsc, boost::system::error_code & err_code);
	// call the right service handler;
	int _call_svc(ip::tcp::socket & clnt, iegad::net::msg_basic & msgbsc);

	// disable @ cpy ctor @ & @ operator= @
	tcp_mt_svr(const tcp_mt_svr &);
	tcp_mt_svr & operator=(const tcp_mt_svr &);
    };

} // namespace net;
} // namespace iegad


#endif // __TCP_MT_SERVER__