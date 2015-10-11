#include "udp_waiter.h"
#include <thread>


#define WAIT_FOR_SINGLE true


iegad::net::udp_waiter::udp_waiter(const std::string & ipstr, unsigned short port, callback_t callback)
    :
    callback_(callback),
    ios_(), 
    sock_(ios_)
{
    boost::system::error_code errcode;
    sock_.open(boost::asio::ip::udp::v4());
    sock_.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(ipstr), port), errcode);
    assert(errcode.value() == 0);
}


int 
iegad::net::udp_waiter::start()
{
    thread_proc_ = std::move(std::thread(std::bind(&iegad::net::udp_waiter::_start, this)));
    thread_proc_.detach();
    return 0;
}


int 
iegad::net::udp_waiter::_start()
{
    char c, cback;
    int n, t = 3;
    boost::system::error_code errcode;
    boost::asio::ip::udp::endpoint rmt_ep;
    
    while (WAIT_FOR_SINGLE) {
	n = sock_.receive_from(boost::asio::buffer(&c, 1), rmt_ep, 0, errcode);
	if (n == 1 && errcode.value() == 0) {
	    cback = c + 1;
	    while (t--) {		
		n = sock_.send_to(boost::asio::buffer(&cback, 1), rmt_ep, 0, errcode);
		if (n == 1 && errcode.value() == 0) {
		    break;
		}
	    } // while (t--);
	    callback_(c);
	} // if (n == 1 && errcode.value() == 0);
    } // while (WAIT_FOR_SINGLE);
}


iegad::net::udp_waiter::~udp_waiter()
{
    sock_.close();
}
