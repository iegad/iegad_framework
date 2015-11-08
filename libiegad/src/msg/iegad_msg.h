#ifndef __IEGAD_MSG__
#define __IEGAD_MSG__



// ============ 说明 ============
//
// @创建日期 : 2015-09-27
// @创建人 : iegad
//
// ============================
// @用途 : 1. 基于 boost::asio 的封装来提供一个 简单的IO操作.
//		 2. 对 msg 的操作封装.
// ============================
//
// @修改记录:
// =======================================
//	    日期          |      修改人         |                         修改说明
// =======================================
//  --2015-10-02	--iegad		-- 将 io操作所产生的err_code 回传给调用者;
//  --2015-10-09	--iegad		-- 注释调 send_n 函数, 改用boost::asio 自带的 send 函数, 先作测试;
//  --2015-10-23	--iegad		-- 将 net 命名空间 改为 msg 命名空间;
//  --2015-11-07     --iegad		-- 1, 去掉 google protocol buffer 库依赖;
//							    2, 去掉protobuf 的 basic_msg 类型;
//							    3, 添加 二进制 消息类型 bmsg_t;
//							    4, 添加 字符串消息和文件消息 的recv/send 操作函数.
//  --2015-11-08	--iegad		-- 添加 加密 发送/接收 函数



#ifdef WIN32

#pragma comment(lib, "libprotoc.lib")
#pragma comment(lib, "libprotobuf.lib")

#endif // WIN32


#include <boost/asio.hpp>
#include <string>
#include "iegad_define.h"


namespace iegad {
namespace msg {


    // ============================
    // @用途 : 从 客户端clnt 读取 字符串消息;
    // @clnt : tcp 客户端
    // @recvbuff : boost::asio::streambuf 缓冲区
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 接收成功返回 客户端传送的字符串消息 , 否则返回 ERR_STRING;
    // ============================
    const std::string recv_str(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff, 
	boost::system::error_code & err_code);

    // ============================
    // @用途 : recv_str => 加密版
    // ============================
    const std::string recv_str(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff, 
	boost::system::error_code & err_code, char msg_key);


    // ============================
    // @用途 : 向 客户端clnt 发送 字符串消息;
    // @clnt : tcp 客户端
    // @msgstr : 需要发送的 字符串消息;
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 成功发送返回 字符串消息长度, 否则返回 -1 并将 设置err_code;
    // ============================
    int send_str(boost::asio::ip::tcp::socket & clnt, const std::string & msgstr, 
	boost::system::error_code & err_code);

    // ============================
    // @用途 : send_str => 加密版
    // ============================
    int send_str(boost::asio::ip::tcp::socket & clnt, const std::string & msgstr, 
	boost::system::error_code & err_code, char msg_key);


    // ============================
    // @用途 : 接收 客户端clnt 发送的 文件, 并写到 指定的 filename文件
    // @clnt : tcp 客户端
    // @filename : 文件完整路径 + 文件名
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 成功发送返回 接收的文件长度 , 否则返回 -1, 并置err_code;
    // ============================
    int recv_file(boost::asio::ip::tcp::socket & clnt, const std::string & filename, boost::system::error_code & err_code);


    // ============================
    // @用途 : 向 客户端clnt 发送的 文件
    // @clnt : tcp 客户端
    // @filename : 文件完整路径 + 文件名
    // @err_code : 回传给调用者的 err_code
    // @返回值 : 成功发送返回 发送的文件长度 , 否则, 返回 -1, 并置err_code;
    // @PS : 当发送完文件之后, 服务端会关闭 发送 通道
    // ============================
    int send_file(boost::asio::ip::tcp::socket & clnt, const std::string & filename, boost::system::error_code & err_code);


} // namespace msg;
} // namespace iegad;



#endif // __IEGAD_MSG__