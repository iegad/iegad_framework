#ifndef __IEGAD_IO_MSG__
#define __IEGAD_IO_MSG__



// ============ 说明 ============
//
// @创建日期 : 2015-09-27
// @创建人 : iegad
//
// ============================
// @用途 : 1. 基于 boost::asio 的封装来提供一个 简单的IO操作.
//		 2. 对 basic_msg 的操作封装.
// ============================
//
// @修改记录:
// =======================================
//	    日期          |      修改人         |                         修改说明
// =======================================
//  --2015-10-02	--iegad		-- 将 io操作所产生的err_code 回传给调用者
//  --2015-10-09	--iegad		-- 注释调 send_n 函数, 改用boost::asio 自带的 send 函数, 先作测试



#ifdef WIN32

#pragma comment(lib, "libprotoc.lib")
#pragma comment(lib, "libprotobuf.lib")

#endif // WIN32


#include <boost/asio.hpp>
#include <string>
#include "msg/basic_msg.pb.h"



namespace iegad {
namespace io {


    // 客户端接收缓冲大小
    enum {
	BUF_SIZE = 1024 * 4
    };
     

    // ============================
    // @用途 : 从 客户端clnt 读数据直到eof为止;
    // @clnt : tcp 客户端
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 返回读取的缓冲区数据
    // ============================
    const std::string recv_end(boost::asio::ip::tcp::socket & clnt, 
	boost::system::error_code & err_code);


 //   // ============================
 //   // @用途 : 向 客户端clnt 发送 send_buf 直到发送buf_size长度为止;
 //   // @clnt : tcp 客户端
 //   // @send_buf : 待发送的数据
 //   // @buf_size, 待发送数据的大小
 //   // @err_code : 回传给调用者的 err_code
 //   // @返回值 : 返回发送的字节数
 //   // ============================
 //   int send_n(boost::asio::ip::tcp::socket & clnt, const char * send_buf, int buf_size, 
	//boost::system::error_code & err_code);


} // namespace net;


namespace net {


    // ============================
    // @用途 : 从 客户端clnt 读取消息 msgbsc;
    // @clnt : tcp 客户端
    // @msgbsk : 需要填充的消息
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 接收成功返回 0 , 否则返回 -1;
    // ============================
    int recv_basic_msg(boost::asio::ip::tcp::socket & clnt, basic_msg & msgbsc, 
	boost::system::error_code & err_code);


    // ============================
    // @用途 : 向 客户端clnt 发送消息 msgbsc;
    // @clnt : tcp 客户端
    // @msgbsc : 需要发送的 basic_msg
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 成功发送返回 0 , 否则返回 -1;
    // ============================
    int send_basic_msg(boost::asio::ip::tcp::socket & clnt, const basic_msg & msgbsc, 
	boost::system::error_code & err_code);


    // ============================
    // @用途 : 向 客户端clnt 发送消息 basic_msg,
    //		    而basic_msg根据type, flag, msg_str来填充;
    // @clnt : tcp 客户端
    // @type : basic_msg.msg_type
    // @flag : basic_msg.msg_flag
    // @msg_bdstr : basic_msg.msg_bdstr
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 成功发送返回 0 , 否则返回 -1;
    // ============================
    int send_basic_msg(boost::asio::ip::tcp::socket & clnt, int type, int flag, const std::string & msg_bdstr, 
	boost::system::error_code & err_code);


} // namespace net;



} // namespace iegad;



#endif // __IEGAD_IO_MSG__