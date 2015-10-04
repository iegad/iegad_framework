#include "tcp_client.h"
#include "common/iegad_log.h"


using namespace iegad::common;


iegad::net::tcp_clnt::tcp_clnt(const std::string & host, std::string svc)
    :ios_(), sock_(ios_), host_(host), svc_(svc)
{}


iegad::net::tcp_clnt::~tcp_clnt()
{
    sock_.close();
}


int 
iegad::net::tcp_clnt::init()
{
    boost::asio::ip::tcp::resolver::iterator end;
    boost::system::error_code errcode = boost::asio::error::host_not_found;
    if (iter_ == end) {
	boost::asio::ip::tcp::resolver rlv(sock_.get_io_service());
	boost::asio::ip::tcp::resolver::query qry(host_, svc_);

	boost::asio::ip::tcp::resolver::iterator iter = rlv.resolve(qry);
	for (; errcode && iter != end; ++iter) {
	    sock_.close();
	    if (sock_.connect(*iter, errcode) == 0) {
		iter_ = iter;
		return 0;
	    }
	}
    }
    else {
	this->_connect(errcode);
    }
    if (errcode) {
	iWARN << errcode.message() << std::endl;
	return -1;
    }
    return 0;
}


int 
iegad::net::tcp_clnt::_connect(boost::system::error_code & ex)
{
    return sock_.connect(*iter_, ex) == 0 ? 0 : -1;
}

boost::asio::ip::tcp::socket & 
iegad::net::tcp_clnt::get()
{
    return sock_;
}

