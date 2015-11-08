#include "nets/basic_svc.h"
#include "common/iegad_string.h"
#include "msg/basic_msg.pb.h"


iegad::nets::basic_svc::basic_svc(int svc_id)
    : 
    svc_id_(svc_id) {}


int 
iegad::nets::basic_svc::get_id()
{
    return svc_id_;
}


int 
iegad::nets::basic_svc::_return(boost::asio::ip::tcp::socket & clnt, const char * rzt, size_t rzt_size, 
    boost::system::error_code & err_code)
{
    return iegad::msg::send_str(clnt, std::string(rzt, rzt_size), err_code, MSG_KEY);
}


int
iegad::nets::basic_svc::_send_msg(boost::asio::ip::tcp::socket & clnt, int flag, 
    const std::string & msg_dbstr, 
    boost::system::error_code & err_code)
{
    std::string msg_str;
    iegad::msg::basic_msg msgbsc;
    msgbsc.set_msg_type(this->get_id());
    msgbsc.set_msg_flag(flag);
    msgbsc.set_msg_bdstr(msg_dbstr);
    if (msgbsc.SerializeToString(&msg_str)) {
	return iegad::msg::send_str(clnt, msg_str, err_code, MSG_KEY) == msg_str.size() + 1 ? 0 : -1;
    }

    return -1;
}

