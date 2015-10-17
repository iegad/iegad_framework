#ifndef __UDP_CLIENT__
#define __UDP_CLIENT__



// ============ 说明 ============
//
// @创建日期 : 2015-10-17
// @创建人 : iegad
//
// ============================
// @用途 : 消息推送接收端
// @PS : 目前使用的可靠UDP通信, 可靠算法可能有问题.
//		目前可能无法实对 多个服务信号端推送一个客户端.
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include <boost/asio.hpp>
#include <functional>
#include <thread>


namespace iegad {
namespace net {


    class udp_puller {
    public:
	typedef std::function<void(char)> callback_t;
	explicit udp_puller(const std::string & ipstr, unsigned short port, callback_t callback);
	~udp_puller();

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