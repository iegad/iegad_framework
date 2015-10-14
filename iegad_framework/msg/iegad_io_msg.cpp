#include "iegad_io_msg.h"
#include "string/iegad_string.h"


const std::string
iegad::io::recv_end(boost::asio::ip::tcp::socket & clnt, 
			     boost::system::error_code & errcode)
{
    int n;
    char recv_buf[BUF_SIZE];
    std::string res;
    char * p = recv_buf;
    do {
	n = clnt.read_some(boost::asio::buffer(recv_buf), errcode);
	if (errcode) {
	    if (errcode == boost::asio::error::interrupted) {
		continue;
	    }
	    break;
	}
	else {
	    res.append(recv_buf, n);
	}
    } while (true);
    return res;
}


//int 
//iegad::io::send_n(boost::asio::ip::tcp::socket & clnt, const char * send_buf, int buf_size, 
//			  boost::system::error_code & errcode)
//{
//    int nleft = buf_size, n;
//    const char * p = send_buf;
//    while (nleft > 0) {
//	n = clnt.write_some(boost::asio::buffer(send_buf, buf_size), errcode);
//	if (errcode) {
//	    if (errcode == boost::asio::error::interrupted) {
//		continue;
//	    } // if (errcode == boost::asio::error::interrupted) 
//	    break;
//	} // if (errcode);
//	nleft -= n;
//    } // while (nleft > 0);
//    return buf_size - nleft;
//}


int 
iegad::net::recv_basic_msg(boost::asio::ip::tcp::socket & clnt, basic_msg & msgbsc, 
					 boost::system::error_code & errcode)
{
    msgbsc.ParseFromString(iegad::string::de_cust(iegad::io::recv_end(clnt, errcode), MSG_KEY));
    return msgbsc.IsInitialized() ? 0 : -1;
}


int 
iegad::net::send_basic_msg(boost::asio::ip::tcp::socket & clnt, const basic_msg & msgbsc, 
					  boost::system::error_code & errcode)
{
    std::string msg_str;
    do {
	if (!msgbsc.IsInitialized()) {
	    break;
	}

	if (!msgbsc.has_msg_bdstr() ||
	    !msgbsc.has_msg_flag() ||
	    !msgbsc.has_msg_type()) {
	    break;
	}

	if (!msgbsc.SerializeToString(&msg_str)) {
	    break;
	}

	//if (iegad::io::send_n(clnt, msg_str.c_str(), msg_str.size(), errcode) != msg_str.size()) {
	//    break;
	//}

	if (clnt.send(boost::asio::buffer(iegad::string::en_cust(msg_str, MSG_KEY)), 0, errcode)
	    != msg_str.size()) {
	    break;
	}

	return 0;
    } while (false);
    return -1;
}


int 
iegad::net::send_basic_msg(boost::asio::ip::tcp::socket & clnt, int type, int flag, const std::string & msg_bdstr, 
					  boost::system::error_code & errcode)
{
    basic_msg msgbsc;
    msgbsc.set_msg_type(type);
    msgbsc.set_msg_flag(flag);
    msgbsc.set_msg_bdstr(msg_bdstr);
    return iegad::net::send_basic_msg(clnt, msgbsc, errcode);
}
