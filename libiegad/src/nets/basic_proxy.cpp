#include "nets/basic_proxy.h"
#include "msg/iegad_msg.h"
#include "common/iegad_string.h"
#include "common/iegad_log.h"
#include "msg/basic_msg.pb.h"


iegad::netc::basic_proxy::~basic_proxy()
{
    if (clnt_.is_open()) {
	clnt_.close();
    }
}


int 
iegad::netc::basic_proxy::_send_basic_msg(int msg_type, int msg_flag, const std::string & msg_bdstr, 
    boost::system::error_code & err_code)
{// func : send the request to the host;    
    iegad::msg::basic_msg msgbsc;
    char * msgdata;
    int datalen, n = -1;

    msgbsc.set_msg_bdstr(msg_bdstr);
    msgbsc.set_msg_flag(msg_flag);
    msgbsc.set_msg_type(msg_type);
    datalen = msgbsc.ByteSize();
    msgdata = new char[datalen];    
    if (msgbsc.SerializeToArray(msgdata, datalen + 1)) {
	n = iegad::msg::send_str(clnt_, 
	    iegad::string::bin_tostr(msgdata, datalen), 
	    err_code) == datalen * 2 && err_code.value() == 0 ? 0 : -1;
    }    
    delete[] msgdata;
    return n;
}


int 
iegad::netc::basic_proxy::_recv_basic_msg(iegad::msg::basic_msg & msgbsc, boost::system::error_code & err_code)
{// func : receive message of protobuf;
    std::string msgstr = iegad::msg::recv_str(clnt_, recvbuff_, err_code);

    if (err_code.value() == 0 && msgstr != ERR_STRING) {
	int len = msgstr.size() / 2;
	char * buff = new char[len];
	if (iegad::string::str_tobin(msgstr, buff, len) != nullptr) {
	    msgbsc.ParseFromArray(buff, len);
	}
	delete[] buff;
    }
    return msgbsc.IsInitialized() ? 0 : -1;
}


const std::string
iegad::netc::basic_proxy::_recv(boost::system::error_code & err_code)
{// func : receive basic message of char buffer;
    std::string res = iegad::msg::recv_str(clnt_, recvbuff_, err_code);
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


int iegad::netc::basic_proxy::_connect()
{// func : connecting the host;
    boost::asio::ip::tcp::resolver::iterator end;
    boost::system::error_code errcode;

    // step 1 : check conn_flag 
    if (conn_flag_) {
	// connect ep, don't resolve the host name & svc name
	clnt_.connect(ep_, errcode);
	if (errcode.value() == 0) {
	    return 0;
	}
	conn_flag_ = false;
    }

    // step 2 : never connected or connected failed,
    //		resolve the host name * svc name;
    errcode = boost::asio::error::host_not_found;
    boost::asio::ip::tcp::resolver rlv(clnt_.get_io_service());
    boost::asio::ip::tcp::resolver::query qry(host_, svc_);

    boost::asio::ip::tcp::resolver::iterator iter = rlv.resolve(qry);
    for (; errcode && iter != end; ++iter) {
	clnt_.close();

	if (clnt_.connect(*iter, errcode) == 0) {
	    // step 3 : find right endpoint & connected sucess, record & return;
	    clnt_.set_option(boost::asio::ip::tcp::no_delay(true));
	    ep_ = *iter;
	    conn_flag_ = true;
	    return 0;
	}
    } // for (; errcode && iter != end; ++iter);

    // step 4 : all failed;
    if (errcode) {
	iWARN << errcode.message() << std::endl;
    }
    return -1;
}


void 
iegad::netc::basic_proxy::close()
{
    boost::system::error_code err_code;
    this->clnt_.close(err_code);
}

iegad::netc::basic_proxy::basic_proxy(const std::string & host, const std::string & svc)
    : 
    ios_(), clnt_(ios_), conn_flag_(false), host_(host), svc_(svc) 
{}

