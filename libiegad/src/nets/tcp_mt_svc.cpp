#include "tcp_mt_svc.h"
#include "common/iegad_string.h"
#include "msg/basic_msg.pb.h"
#include "common/iegad_log.h"



int 
iegad::nets::tcp_mt_svc::_action(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff)
{
    boost::system::error_code err_code;
    std::string msgstr = this->_get_msgstr(clnt, recvbuff, err_code);

    if (msgstr == ERR_STRING || err_code.value() != 0) {
	if (err_code == boost::asio::error::timed_out) {
	    iWARN << clnt.remote_endpoint().address().to_string()
		<< " ### TIME OUT ###" << std::endl;
	}
	else {
	    iWARN << clnt.remote_endpoint().address().to_string()
		<< " ### " << err_code.message() << " ###" << std::endl;
	}
	return -1;
    } // if (msgstr == ERR_STRING || err_code.value() != 0);     
    
    iegad::msg::basic_msg msgbsc;    
    msgbsc.ParseFromString(msgstr);

    if (msgbsc.IsInitialized() &&
	map_.find(msgbsc.msg_type()) != map_.end()) {
	return map_[msgbsc.msg_type()]->invoke(clnt, msgbsc.msg_flag(),
	    msgbsc.msg_bdstr());
    }

    return -1;
}


iegad::nets::tcp_mt_svc::tcp_mt_svc(unsigned short port)
    : iegad::nets::tcp_mt_svr(port)
{}


iegad::nets::tcp_mt_svc::tcp_mt_svc(const std::string & host, const std::string & svc)
    : iegad::nets::tcp_mt_svr(host, svc)
{}


bool 
iegad::nets::tcp_mt_svc::regist_svc(basic_svc::basic_svc_ptr svc)
{
    return map_.insert(map_t::value_type(svc->get_id(), svc)).second;
}


const std::string 
iegad::nets::tcp_mt_svc::_get_msgstr(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff, 
	boost::system::error_code & err_code)
{
    std::string res;
    res = iegad::msg::recv_str(clnt, recvbuff, err_code);
    int len = res.size() / 2;
    char * buff = new char[len];
    if (iegad::string::str_tobin(res, buff, len) != nullptr) {
	res = std::move(std::string(buff, len));
    }
    else {
	res = ERR_STRING;
    }
    delete[] buff;
    return res;
}


