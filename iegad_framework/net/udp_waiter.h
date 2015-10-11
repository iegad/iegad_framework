#ifndef __UDP_CLIENT__
#define __UDP_CLIENT__


#include <boost/asio.hpp>
#include <functional>
#include <thread>


namespace iegad {
namespace net {


    class udp_waiter {
    public:
	typedef std::function<void(char)> callback_t;
	explicit udp_waiter(const std::string & ipstr, unsigned short port, callback_t callback);
	~udp_waiter();

	int start();
	
    private:
	int _start();

	callback_t callback_;
	boost::asio::io_service ios_;
	boost::asio::ip::udp::socket sock_;
	std::thread thread_proc_;
    };


} // namespace net;
} // namespace iegad;


#endif // __UDP_CLIENT__