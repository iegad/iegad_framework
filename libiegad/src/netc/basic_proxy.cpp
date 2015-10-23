#include "nets/basic_proxy.h"
#include "msg/iegad_io_msg.h"
#include "common/iegad_string.h"
#include "common/iegad_log.h"


iegad::netc::basic_proxy::~basic_proxy()
{
    if (clnt_.is_open()) {
	clnt_.close();
    }
}


int iegad::netc::basic_proxy::_send_msg(int msg_type, int msg_flag, const std::string & msg_bdstr)
{// func : send the request to the host;
    boost::system::error_code err_code;
    int n = iegad::msg::send_basic_msg(clnt_, msg_type, msg_flag, msg_bdstr, err_code);
    if (n == 0) {
	// shutdown send route when sended sucess;
	clnt_.shutdown(boost::asio::socket_base::shutdown_send);
    }
    return n;
}


int iegad::netc::basic_proxy::_recv_msg(iegad::msg::basic_msg & msgbsc, boost::system::error_code & err_code)
{// func : receive message of protobuf;
    return iegad::msg::recv_basic_msg(clnt_, msgbsc, err_code);
}


const std::string
iegad::netc::basic_proxy::_recv()
{// func : receive basic message of char buffer;
    boost::system::error_code err_code;
    char buff[BUF_SIZE];
    std::string res;
    int n;
    do {
	n = clnt_.read_some(boost::asio::buffer(buff), err_code);
	if (err_code) {
	    if (err_code == boost::asio::error::eof) {
		break;
	    }
	    else if (err_code == boost::asio::error::interrupted){
		continue;
	    }
	    else {
		res = "";
		break;
	    }
	}
	res.append(buff, n);
    } while (true);
    return iegad::string::de_cust(res, MSG_KEY);
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