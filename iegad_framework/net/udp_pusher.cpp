#include "udp_pusher.h"
#include <assert.h>
#include <thread>
#include "common/iegad_log.h"


using namespace boost::asio;
using namespace boost::asio::ip;





int 
iegad::net::udp_svr::send_all(const char * buff, int buff_size)
{
    int res = remotes_.size();
    remotes_t::iterator itor = remotes_.begin();
    for (; itor != remotes_.end(); itor++) {
	io_service ios;
	udp::socket sock(ios);
	do {
	    if (this->_build_sock(sock) != 0) {
		break;
	    }

	    if (this->_send(sock, itor->first, buff, buff_size) != 0) {
		break;
	    }
	    continue;
	} while (false);
	res--;
    } // for (; itor != remotes_.end(); itor++);
    return res;
}


int 
iegad::net::udp_svr::_build_sock(boost::asio::ip::udp::socket & sock)
{
    boost::system::error_code errcode;
    sock.open(udp::v4());
    for (int i = UDP_SVR_PORT;; i++) {
	sock.bind(udp::endpoint(address::from_string(ip_), i), errcode);
	if (errcode.value() == 0) {
	    break;
	}
    }
    return 0;
}


iegad::net::udp_svr::udp_svr(const std::string & ipstr)
    :
    ip_(ipstr)
{}


int 
iegad::net::udp_svr::_send(boost::asio::ip::udp::socket & sock, 
const std::string & rmt_id, 
const char * buff, int buff_size)
{
    boost::system::error_code errcode;
    int n, timeout = 1;

    remotes_t::iterator itor = remotes_.find(rmt_id);
    if (itor == remotes_.end()) {
	return -1;
    }

    sock.connect(itor->second, errcode);
    if (errcode == error::network_unreachable) {
	this->rm_client(rmt_id);
	return -1;
    }

    while (timeout < MAX_TIMEOUT_VALUE) {
	n = setsockopt(sock.native(), SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(int));
	n = sock.send(buffer(buff, buff_size), 0, errcode);
	if (n == buff_size && errcode.value() == 0) {
	    char c;
	    n = sock.receive(buffer(&c, 1), 0, errcode);
	    if (n == 1 && errcode.value() == 0 && c == buff[0] + 1) {
		return 0;
	    } 
	} 
	if (errcode == error::connection_reset) {
	    this->rm_client(rmt_id);
	    return -1;
	}
	iWARN << errcode.message() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(timeout));
	timeout *= 2;
    } // while (t-- > 0);
    return -1;
}


int 
iegad::net::udp_svr::send_one(const std::string & rmt_id, const char * buff, int buff_size)
{
    io_service ios;
    udp::socket sock(ios);

    if (this->_build_sock(sock) != 0) {
	return -1;
    }

    return this->_send(sock, rmt_id, buff, buff_size);
}


int 
iegad::net::udp_svr::add_client(const std::string & rmt_id, rmt_addr_t & rmt_addr)
{
    remotes_t::iterator itor = remotes_.find(rmt_id);
    if (itor != remotes_.end()) {
	return -1;
    }
    remotes_[rmt_id] = udp::endpoint(address::from_string(rmt_addr.first), rmt_addr.second);
    return 0;
}


int 
iegad::net::udp_svr::rm_client(const std::string & rmt_id)
{
    remotes_t::iterator itor = remotes_.find(rmt_id);
    if (itor == remotes_.end()) {
	return -1;
    }
    remotes_.erase(itor);
    return 0;
}


iegad::net::udp_svr::~udp_svr()
{

}

