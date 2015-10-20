#include "udp_puller.h"
#include <thread>
#include "iegad_define.h"


iegad::net::udp_puller::udp_puller(const std::string & ipstr, unsigned short port, callback_t callback)
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
iegad::net::udp_puller::start()
{
    thread_proc_ = std::move(std::thread(std::bind(&iegad::net::udp_puller::_start, this)));
    thread_proc_.detach();
    return 0;
}


int 
iegad::net::udp_puller::_start()
{
    char c, rc = -1;
    int n, t = 3;
    boost::system::error_code errcode;
    boost::asio::ip::udp::endpoint rmt_ep, rcd_ep;
    
    while (WAIT_FOR_SINGLE) {
	n = sock_.receive_from(boost::asio::buffer(&c, 1), rmt_ep, 0, errcode);
	if (n == 1 && errcode.value() == 0) {
	    if (SET_UDP_FLAG(c) == rc && rcd_ep == rmt_ep) {
		continue;
	    }
	    rc = SET_UDP_FLAG(c);
	    rcd_ep = rmt_ep;
	    while (t--) {		
		n = sock_.send_to(boost::asio::buffer(&rc, 1), rmt_ep, 0, errcode);
		if (n == 1 && errcode.value() == 0) {
		    break;
		}
	    } // while (t--);
	    callback_(c);	    
	} // if (n == 1 && errcode.value() == 0);
    } // while (WAIT_FOR_SINGLE);
}


iegad::net::udp_puller::~udp_puller()
{
    sock_.close();
}
