#ifndef __IEGAD_IO_MSG__
#define __IEGAD_IO_MSG__



// ============ 说明 ============
//
// @创建日期 : 2015-09-27
// @创建人 : iegad
//
// ============================
// @用途 : 1. 基于 boost::asio 的封装来提供一个 简单的IO操作.
//		 2. 对 msg_basic 的操作封装.
// ============================
//
// @修改记录:
// =======================================
//	    日期          |      修改人         |                         修改说明
// =======================================
//  --2015-10-02	--iegad		-- 将 io操作所产生的err_code 回传给调用者



#ifdef WIN32

#pragma comment(lib, "libprotoc.lib")
#pragma comment(lib, "libprotobuf.lib")

#endif // WIN32


#include <boost/asio.hpp>
#include <string>
#include "msg_basic.pb.h"



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
    const std::string recv_end(boost::asio::ip::tcp::socket & clnt, boost::system::error_code & err_code);


    // ============================
    // @用途 : 向 客户端clnt 发送 send_buf 直到发送buf_size长度为止;
    // @clnt : tcp 客户端
    // @send_buf : 待发送的数据
    // @buf_size, 待发送数据的大小
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 返回发送的字节数
    // ============================
    int send_n(boost::asio::ip::tcp::socket & clnt, const char * send_buf, int buf_size, boost::system::error_code & err_code);


} // namespace net;


namespace net {


    // ============================
    // @用途 : 从 客户端clnt 读取消息 msgbsc;
    // @clnt : tcp 客户端
    // @msgbsk : 需要填充的消息
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 接收成功返回 0 , 否则返回 -1;
    // @PS : 如明果 timeout 不为 0, 则会为clnt 设定接收超时, 
    //		 否则, 接收将永久阻塞.
    // ============================
    int recv_msg_basic(boost::asio::ip::tcp::socket & clnt, msg_basic & msgbsc, boost::system::error_code & err_code);


    // ============================
    // @用途 : 向 客户端clnt 发送消息 msgbsc;
    // @clnt : tcp 客户端
    // @msgbsc : 需要发送的 msg_basic
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 成功发送返回 0 , 否则返回 -1;
    // ============================
    int send_msg_basic(boost::asio::ip::tcp::socket & clnt, const msg_basic & msgbsc, boost::system::error_code & err_code);


    // ============================
    // @用途 : 向 客户端clnt 发送消息 msg_basic,
    //		    而msg_basic根据type, flag, msg_str来填充;
    // @clnt : tcp 客户端
    // @type : msg_basic.msg_type
    // @flag : msg_basic.msg_flag
    // @msg_bdstr : msg_basic.msg_bdstr
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 成功发送返回 0 , 否则返回 -1;
    // ============================
    int send_msg_basic(boost::asio::ip::tcp::socket & clnt, int type, int flag, const std::string & msg_bdstr, boost::system::error_code & err_code);


} // namespace net;



} // namespace iegad;



#endif // __IEGAD_IO_MSG__