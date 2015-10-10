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
	clnt_.close();
	return "";
    }

    std::string res;

    if (this->_send_msg(10, 10, echo_str) != 0) {
	clnt_.close();
	return "";
    }

    res = this->_recv();
    clnt_.close();
    return res;
}


echo_proxy::echo_proxy(const std::string & host, const std::string & svc)
    : 
    basic_proxy(host, svc) {}
