#include "echo_svc.h"


int 
iegad::net::echo_svc::action(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr)
{
    if (this->_response(clnt, msg_bdstr.c_str(),
	msg_bdstr.size()) != msg_bdstr.size()) {
	iERR << "echo_msg responce failed" << std::endl;
    }
    return 0;
}

iegad::net::echo_svc::echo_svc(int svc_id)
    :
    iegad::net::basic_svc(svc_id)
{}
