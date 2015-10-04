#include "echo_svc.h"


int 
iegad::net::echo_svc::action(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr)
{
    iegad::net::echo_msg msgdata_;
    if (this->_build_svc<iegad::net::echo_msg>(msg_bdstr, msgdata_) != 0) {
	iERR << "echo_msg build failed" << std::endl;
	return -1;
    }
    if (this->_response(clnt, msgdata_.requ_str().c_str(),
	msgdata_.requ_str().size()) != msgdata_.requ_str().size()) {
	iERR << "echo_msg responce failed" << std::endl;
    }
    return 0;
}

iegad::net::echo_svc::echo_svc(int svc_id)
    :
    iegad::net::svc_basic(svc_id)
{}
