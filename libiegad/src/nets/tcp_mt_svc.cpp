#include "tcp_mt_svc.h"
#include "common/iegad_string.h"
#include "msg/basic_msg.pb.h"



int 
iegad::nets::tcp_mt_svc::_action(boost::asio::ip::tcp::socket & clnt, const std::string & msgstr)
{
    iegad::msg::basic_msg msgbsc;
    msgbsc.ParseFromString(msgstr);
    if (msgbsc.IsInitialized() && map_.find(msgbsc.msg_type()) != map_.end()) {
	return map_[msgbsc.msg_type()]->invoke(clnt, msgbsc.msg_flag(), msgbsc.msg_bdstr());
    }
    return -1;
}


iegad::nets::tcp_mt_svc::tcp_mt_svc(unsigned short port)
    : iegad::nets::tcp_mt_svr(port)
{}


iegad::nets::tcp_mt_svc::tcp_mt_svc(const std::string & host, const std::string & svc)
    : iegad::nets::tcp_mt_svr(host, svc)
{}


int 
iegad::nets::tcp_mt_svc::regist_svc(basic_svc::basic_svc_ptr svc)
{
    if (map_.find(svc->get_id()) != map_.end()) {
	return -1;
    }
    map_[svc->get_id()] = svc;
    return 0;
}

