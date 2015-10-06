#ifndef __PROXY_BASIC__
#define __PROXY_BASIC__


// ============ 说明 ============
//
// @创建日期 : 2015-10-04
// @创建人 : iegad
//
// ============================
// @用途 :  该类为虚基类, 
//		  作用是提供客户端调用 服务对象 的代理, 实现在了一个统一调用的接口.
//		  代理的实现, 使用 重载 operator() 运算符,
//		  让它使用起来更像是一个函数对象, 该类用作 客户端代理基类
// @PS : 客户端在建立 客户端代理 与 服务对象 的通信时, 应基于该类
//		创建出一个适合自己的服务对象所对应的代理类
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================


#include <boost/asio.hpp>
#include "net/iegad_io_msg.h"


namespace iegad {
namespace net {


    // ============================
    // @__R :  operator() 返回值类型
    // @__P : operator() 参数类型
    // @__MSG_T : pb所生成的消息类型
    // ============================
    template <typename __R, typename __P, typename __MSG_T>
    class proxy_basic {
    // 客户代理基类
    public:


	// ============================
	// @用途 : 构造函数
	// @host : 服务端机器名或IP地址
	// @svc : 服务名或端口号
	// ============================
	explicit proxy_basic(const std::string & host, const std::string & svc)
	    : ios_(), clnt_(ios_), host_(host), svc_(svc) {}


	// ============================
	// @用途 : 析构函数
	// @PS : 该函数为关闭 clnt_ 的套接字对象
	// ============================
	virtual ~proxy_basic();


	// ============================
	// @用途 : 远程服务对象调用接口
	// @param : 调用参数, 如果需要传递多个参数, 需要定义一个结构体来传参
	// @返回值 : 模板的 __R 类型, 返回值, 由派生类自行定义规则.
	// ============================
	virtual const __R operator()(const __P & param) = 0;

    protected:
	// ============================
	// @用途 : 远程服务对象调用接口
	// @param : 调用参数, 如果需要传递多个参数, 需要定义一个结构体来传参
	// @返回值 : 成功发送返回0, 否则返回 -1.
	// ============================
	int _send_msg(int msg_type, int msg_flag);


	// ============================
	// @用途 : 接收响应的消息, 将填充一个 msg_basic 类型对象
	// @msgbsc : 用来接收的msg_basic 对象
	// @err_code : 发生错误时, 用来接收错误信息
	// @返回值 : 成功返回0, 否则返回 -1.
	// ============================
	int _recv_msg(msg_basic & msgbsc, boost::system::error_code & err_code);


	// ============================
	// @用途 : 接收响应的消息, 消息类型为字符串型式的基础类型, 
	//		可以通过转换函数, 将返回值转换成预期的类型
	// @返回值 : 成功返回对应类型的字符串形式, 否则, 返回 "" 一个空字符串.
	// ============================
	const std::string _recv();


	// ============================
	// @用途 : 连接服务端进程
	// @返回值 : 成功连接, 返回0, 否则, 返回 -1
	// ============================
	int _connect();


	// boost::asio::io_service 对象
	boost::asio::io_service ios_;
	// boost套接字对象
	boost::asio::ip::tcp::socket clnt_;
	// pb 消息类型
	__MSG_T msg_;

    private:
	// 服务端机器名, 或IP
	std::string host_;
	// 服务名或端口
	std::string svc_;

	// 禁用
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
    int iegad::net::proxy_basic<__R, __P, __MSG_T>::_recv_msg(msg_basic & msgbsc, boost::system::error_code & err_code)
    {
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