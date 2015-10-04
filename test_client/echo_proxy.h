#ifndef __ECHO_PROXY__
#define __ECHO_PROXY__


#include "iegad_framework.h"
#include "tcp_client.h"
#include "echo_msg.pb.h"
#include "proxy_basic.hpp"


using namespace iegad::net;


class echo_proxy : proxy_basic<std::string, std::string, echo_msg> {
public:
    explicit echo_proxy(iegad::net::tcp_clnt & clnt) 
	: proxy_basic(clnt) {}

    const std::string operator()(const std::string & str) {
	return _echo_svc_proc(str);
    }

private:
    const std::string _echo_svc_proc(const std::string & echo_str) {
	if (clnt_.init() != 0) {
	    return "";
	}

	int n;
	std::string res;
	msg_.set_requ_str(echo_str.c_str());

	if (this->_send_msg(10, 10) != 0) {
	    return "";
	}

	res = this->_recv();
	clnt_.get().close();
	return res;
    }
};


#endif // __ECHO_PROXY__