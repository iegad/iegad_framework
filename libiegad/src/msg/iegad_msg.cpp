#include "iegad_msg.h"
#include "common/iegad_string.h"
#include <fstream>


const std::string 
iegad::msg::recv_str(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff,
    boost::system::error_code & err_code)
{
    int n = boost::asio::read_until(clnt, recvbuff, '\0', err_code);
    if (err_code) {
	return ERR_STRING;
    }
    
    std::string res( // n - 1 : 表示丢弃 '\0'
	boost::asio::buffer_cast<const char *>(recvbuff.data()), n - 1);
    // 清除 缓冲区内所使用的数据
    recvbuff.consume(n);
    return res;
}


const std::string 
iegad::msg::recv_str(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff, 
    boost::system::error_code & err_code, char msg_key)
{
    std::string res = iegad::msg::recv_str(clnt, recvbuff, err_code);
    return iegad::string::de_cust(res, msg_key);
}


int 
iegad::msg::send_str(boost::asio::ip::tcp::socket & clnt, const std::string & msgstr, 
    boost::system::error_code & err_code)
{
    // msgstr.size() + 1 : 连同 '\0' 一起发送
    int n = clnt.write_some(boost::asio::buffer(msgstr.c_str(), msgstr.size() + 1), err_code);
    return err_code ? -1 : n;
}


int 
iegad::msg::send_str(boost::asio::ip::tcp::socket & clnt, const std::string & msgstr, 
    boost::system::error_code & err_code, char msg_key)
{
    return send_str(clnt, iegad::string::en_cust(msgstr, msg_key), err_code);
}


int 
iegad::msg::recv_file(boost::asio::ip::tcp::socket & clnt, const std::string & filename, 
    boost::system::error_code & err_code)
{
    int n = 0, res = 0;
    char recvbuff[BUF_SIZE];
    std::ofstream fout(filename, std::ios_base::binary | std::ios_base::out);

    do {
	if (fout.fail() || !fout || !fout.is_open()) {
	    break;
	}

	while (true) {
	    n = clnt.read_some(boost::asio::buffer(recvbuff), err_code);
	    if (err_code) {
		if (err_code == boost::asio::error::interrupted) {
		    continue;
		}
		break;
	    }
	    else {
		fout.write(recvbuff, n);
		res += n;
	    }
	} // while (true);
    } while (false);
    fout.close();
    return err_code == boost::asio::error::eof ? n : -1;
}


int 
iegad::msg::send_file(boost::asio::ip::tcp::socket & clnt, const std::string & filename, boost::system::error_code & err_code)
{
    int n = 0, res = -1;
    char sendbuff[BUF_SIZE];
    std::ifstream fin(filename, std::ios_base::binary | std::ios_base::in);

    do {
	if (fin.fail() || !fin || !fin.is_open()) {
	    break;
	}

	res = 0;
	while (true) {
	    fin.read(sendbuff, BUF_SIZE);
	    n = fin.gcount();
	    again:
	    n = clnt.write_some(boost::asio::buffer(sendbuff, n), err_code);
	    res += n;
	    if (err_code) {
		if (err_code == boost::asio::error::interrupted) {
		    goto again;
		}
		break;
	    }
	    if (fin.eof()) {
		break;
	    }
	} // while (fin.read(sendbuff, BUF_SIZE));
	clnt.shutdown(boost::asio::socket_base::shutdown_send);
    } while (false);
    fin.close();
    return err_code ? -1 : res;
}







