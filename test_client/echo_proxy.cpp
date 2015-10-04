#include "echo_proxy.h"


const std::string 
echo_proxy::operator()(const std::string & param)
{
    return this->_echo_svc_proc(param);
}


const std::string 
echo_proxy::_echo_svc_proc(const std::string & echo_str)
{
    if (this->_connect() != 0) {
	return "";
    }

    int n;
    std::string res;
    msg_.set_requ_str(echo_str.c_str());

    if (this->_send_msg(10, 10) != 0) {
	return "";
    }

    res = this->_recv();
    clnt_.close();
    return res;
}


echo_proxy::echo_proxy(const std::string & host, const std::string & svc)
    : 
    proxy_basic(host, svc) {}
