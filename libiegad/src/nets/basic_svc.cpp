﻿#include "nets/basic_svc.h"
#include "common/iegad_string.h"
#include "msg/basic_msg.pb.h"
#include "common/iegad_compress.hpp"
#include "iegad_msg_ex.h"


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
    //std::string temp = iegad::string::bin_tostr(rzt, rzt_size);
    //std::string msgstr;
    //size_t n = iegad::tools::compress(temp.c_str(), temp.size(), &msgstr);
    //return iegad::msg::send_str(clnt, msgstr, err_code);
    return iegad::msg::msg_ex::send_data(clnt, rzt, rzt_size, err_code);
}


int
iegad::nets::basic_svc::_send_msg(boost::asio::ip::tcp::socket & clnt, int flag, 
    const std::string & msg_dbstr, 
    boost::system::error_code & err_code)
{
    int n = -1;
    iegad::msg::basic_msg msgbsc;
    msgbsc.set_msg_type(this->get_id());
    msgbsc.set_msg_flag(flag);
    msgbsc.set_msg_bdstr(msg_dbstr);
    int datalen = msgbsc.ByteSize();
    char * msgdata = new char[datalen];
    if (msgbsc.SerializeToArray(msgdata, datalen)) {
	n = iegad::msg::msg_ex::send_data(clnt, msgdata, datalen, err_code);
    }
    delete[] msgdata;
    return n;
}

