#ifndef __TCP_MT_SERVER__
#define __TCP_MT_SERVER__


#include <boost/asio.hpp>


#include <mutex>
#include <vector>
#include <thread>
#include <unordered_map>
#include <functional>


#include "service_basic.h"


namespace iegad {
namespace net {


    using namespace boost::asio;


    class tcp_mt_svr {
    // 多线程并发服务
    enum { BUF_SIZE = (1024 * 16) };

    public:
	typedef std::vector<std::thread> thread_pool_t;
	typedef std::unique_lock<std::mutex> mtx_lock_t;
	typedef std::unordered_map<int, svc_basic_ptr> svc_t;

	explicit tcp_mt_svr(
	    const std::string & host,
	    unsigned short port);

	~tcp_mt_svr() {};

	void run(int n = 8);

	void stop();

	void regist_svc(svc_basic_ptr svc_obj);

    protected:
	bool get_stop();


    private:
	bool stop_flag_;
	io_service ios_;
	ip::tcp::acceptor acptor_;
	thread_pool_t thread_pool_;
	std::mutex thread_mtx_;
	std::mutex stop_mtx_;
	svc_t svc_map_;

	void _thread_proc();
	int _accept(ip::tcp::socket & clnt, boost::system::error_code & err_code);

	tcp_mt_svr(const tcp_mt_svr &);
	tcp_mt_svr & operator=(const tcp_mt_svr &);
    };

} // namespace net;
} // namespace iegad


#endif // __TCP_MT_SERVER__