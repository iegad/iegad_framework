#ifndef __BASIC_PROXY__
#define __BASIC_PROXY__


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
// -- 2015-10-09	    --iegad				-- 为了提高性能, 发现 子pb消息类型 可以去除, 改用自定义的序列化对象方式,
//									    去掉 子pb类型, 直接传递字符串, 这样提供了上层更多的控制, 是传递文本消息, 
//									    还是传递一个对象的序列化. 同时也减少了一次序列化的过程.
// -- 2015-10-22	    --iegad				-- 弃用 模板参数, 目的是为了让使用更佳清晰
// -- 2015-11-08	    --iegad				-- 1, 替换 新版 msg 收/发 函数
//									    2, 添加close() 函数


#include <boost/asio.hpp>
#include "msg/basic_msg.pb.h"


namespace iegad {
namespace netc {


    class basic_proxy {
    // 客户代理基类
    public:
	// ============================
	// @用途 : 构造函数
	// @host : 服务端机器名或IP地址
	// @svc : 服务名或端口号
	// @PS : 经过测试, 如果host 传递为计算机名, 解析时, 会占用比较长的时间
	//		所以, host 参数尽可能的使用 ip 地址.
	// ============================
	explicit basic_proxy(const std::string & host, const std::string & svc)
	    : ios_(), clnt_(ios_), conn_flag_(false), host_(host), svc_(svc) {}


	// ============================
	// @用途 : 析构函数
	// @PS : 该函数为关闭 clnt_ 的套接字对象
	// ============================
	virtual ~basic_proxy();


	// ============================
	// @用途 : 远程服务对象调用接口
	// @param : 调用参数, 如果需要传递多个参数, 需要定义一个结构体来传参
	// @返回值 : 模板的 __R 类型, 返回值, 由派生类自行定义规则.
	// ============================
	virtual const  std::string call(const std::string & msgstr) = 0;


	// ============================
	// @用途 : 关闭连接
	// @返回值 : void
	// ============================
	void close();


    protected:
	// ============================
	// @用途 : 远程服务对象调用接口
	// @param : 调用参数, 如果需要传递多个参数, 需要定义一个结构体来传参
	// @返回值 : 成功发送返回0, 否则返回 -1.
	// ============================
	int _send_basic_msg(int msg_type, int msg_flag, const std::string & msg_dbstr);


	// ============================
	// @用途 : 接收响应的消息, 将填充一个 basic_msg 类型对象
	// @msgbsc : 用来接收的basic_msg 对象
	// @err_code : 发生错误时, 用来接收错误信息
	// @返回值 : 成功返回0, 否则返回 -1.
	// ============================
	int _recv_basic_msg(iegad::msg::basic_msg & msgbsc, boost::system::error_code & err_code);


	// ============================
	// @用途 : 接收响应的消息, 消息类型为字符串型式的基础类型, 
	//		可以通过转换函数, 将返回值转换成预期的类型
	// @err_code : 错误信息
	// @返回值 : 成功返回对应类型的字符串形式, 否则, 返回 "" 一个空字符串.
	// ============================
	const std::string _recv(boost::system::error_code & err_code);


	// ============================
	// @用途 : 连接服务端进程
	// @返回值 : 成功连接, 返回0, 否则, 返回 -1
	// ============================
	int _connect();


	// boost::asio::io_service 对象
	boost::asio::io_service ios_;
	// boost套接字对象
	boost::asio::ip::tcp::socket clnt_;

    private:
	// 连接标记
	bool conn_flag_;
	// 服务端机器名, 或IP
	std::string host_;
	// 服务名或端口
	std::string svc_;
	// boost::asio::streambuf 缓冲区
	boost::asio::streambuf recvbuff_;
	// 服务端终结点
	boost::asio::ip::tcp::endpoint ep_;


	// 禁用
	basic_proxy(const basic_proxy &);
	basic_proxy & operator=(const basic_proxy &);

    }; // class basic_proxy;
    

} // namespace net;
} // namespace iegad;



#endif // __BASIC_PROXY__