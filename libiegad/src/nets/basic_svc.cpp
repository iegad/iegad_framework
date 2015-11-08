#include "nets/basic_svc.h"
#include "common/iegad_string.h"
#include "msg/basic_msg.pb.h"


iegad::nets::basic_svc::basic_svc(int svc_id)
    : svc_id_(svc_id) {}


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

