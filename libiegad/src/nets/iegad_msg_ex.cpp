#include "iegad_msg_ex.h"
#include "msg/iegad_msg.h"
#include "common/iegad_string.h"
#include "common/iegad_compress.hpp"



const std::string
iegad::msg::msg_ex::recv_data(boost::asio::ip::tcp::socket & clnt, 
    boost::asio::streambuf & recvbuff, 
    boost::system::error_code & err_code)
{
    std::string temp = iegad::msg::recv_str(clnt, recvbuff, err_code);
    std::string data;

    if (err_code.value() != 0 && temp == ERR_STRING) {
	return ERR_STRING;
    }
    int len = temp.size() / 2;
    char * buff = new char[len];
    if (iegad::string::str_tobin(temp, buff, len) == nullptr) {
	delete[] buff;
	return ERR_STRING;
    }

    if (!iegad::tools::uncompress(buff, len, &data)) {
	delete[] buff;
	return ERR_STRING;
    }
    delete[] buff;
    return data;
}


int 
iegad::msg::msg_ex::send_data(boost::asio::ip::tcp::socket & clnt, 
    const char * data, 
    size_t data_size, 
    boost::system::error_code & err_code)
{
    std::string temp;
    size_t n = iegad::tools::compress(data, data_size, &temp);
    std::string msgstr = iegad::string::bin_tostr(temp.c_str(), temp.size());

    return iegad::msg::send_str(clnt, msgstr, err_code);
}

