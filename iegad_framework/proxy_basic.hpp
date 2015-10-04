#ifndef __PROXY_BASIC__
#define __PROXY_BASIC__


#include <boost/asio.hpp>
#include "net/iegad_io_msg.h"


namespace iegad {
namespace net {


    template <typename __R, typename __P, typename __MSG_T>
    class proxy_basic {
    // 远程代理基类
    public:
	explicit proxy_basic(const std::string & host, const std::string & svc)
	    : ios_(), clnt_(ios_), host_(host), svc_(svc) {}

	virtual ~proxy_basic();

	virtual const __R operator()(const __P & param) = 0;

    protected:
	int _send_msg(int msg_type, int msg_flag);
	int _recv_msg();
	const std::string _recv();
	int _connect();

	boost::asio::io_service ios_;
	boost::asio::ip::tcp::socket clnt_;
	__MSG_T msg_;

    private:
	std::string host_;
	std::string svc_;
	proxy_basic(const proxy_basic &);
	proxy_basic & operator=(const proxy_basic &);

    }; // class proxy_basic<__R, __P, __MSG_T>;



// ============================ 以下是实现部分 ============================



    template <typename __R, typename __P, typename __MSG_T>
    iegad::net::proxy_basic<__R, __P, __MSG_T>::~proxy_basic()
    {
	if (clnt_.is_open()) {
	    clnt_.close();
	}
    }


    template <typename __R, typename __P, typename __MSG_T>
    int iegad::net::proxy_basic<__R, __P, __MSG_T>::_send_msg(int msg_type, int msg_flag)
    {
	boost::system::error_code err_code;
	std::string msg_bdstr;
	if (!msg_.SerializeToString(&msg_bdstr)) {
	    return -1;
	}
	int n = iegad::net::send_msg_basic(clnt_, msg_type, msg_flag, msg_bdstr, err_code);
	if (n == 0) {
	    clnt_.shutdown(boost::asio::socket_base::shutdown_send);
	}
	return n;
    }


    template <typename __R, typename __P, typename __MSG_T>
    int iegad::net::proxy_basic<__R, __P, __MSG_T>::_recv_msg()
    {
	msg_basic msgbsc;
	boost::system::error_code err_code;
	return iegad::net::recv_msg_basic(clnt_, msgbsc, err_code);
    }


    template <typename __R, typename __P, typename __MSG_T>
    const std::string iegad::net::proxy_basic<__R, __P, __MSG_T>::_recv()
    {
	boost::system::error_code err_code;
	char buff[iegad::io::BUF_SIZE];
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
	return res;
    }


    template <typename __R, typename __P, typename __MSG_T>
    int iegad::net::proxy_basic<__R, __P, __MSG_T>::_connect()
    {
	boost::asio::ip::tcp::resolver::iterator end;
	boost::system::error_code errcode = boost::asio::error::host_not_found;


	boost::asio::ip::tcp::resolver rlv(clnt_.get_io_service());
	boost::asio::ip::tcp::resolver::query qry(host_, svc_);

	boost::asio::ip::tcp::resolver::iterator iter = rlv.resolve(qry);
	for (; errcode && iter != end; ++iter) {
	    clnt_.close();
	    if (clnt_.connect(*iter, errcode) == 0) {
		return 0;
	    }
	} // for (; errcode && iter != end; ++iter);

	if (errcode) {
	    iWARN << errcode.message() << std::endl;
	}
	return -1;
    }





} // namespace net;
} // namespace iegad;



#endif // __PROXY_BASIC__