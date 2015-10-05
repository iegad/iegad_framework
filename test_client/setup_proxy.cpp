#include "setup_proxy.h"


setup_proxy::setup_proxy(const std::string & host, const std::string & svc)
    : iegad::net::proxy_basic<int, setup_t, setup_msg>(host, svc)
{}


const int 
setup_proxy::operator()(const setup_t & param)
{
    return this->_setup(param);
}


int 
setup_proxy::_setup(const setup_t & param)
{
    if (this->_connect() != 0) {
	return -1;
    }
    std::string rzt;
    msg_.set_log_id(param.log_id);
    msg_.set_pwd(param.pwd);
    msg_.set_email(param.email);
    if (this->_send_msg(11, 0) != 0) {
	return -1;
    }

    if ((rzt = this->_recv()) == "") {
	return -1;
    }
    return iegad::string::to_int32(rzt);
}
