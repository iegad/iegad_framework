#ifndef __IEGAD_TCP_EVENT__
#define __IEGAD_TCP_EVENT__


// ============================================
// @说明:
//      Tcp事件抽象类,
//      使用该框架的服务必需实现该类
//
// @作者: iegad
// @时间: 2019-06-30
// ============================================
//     时间               修改人                 修改说明
// ============================================


#include <iegad_net.hpp>


namespace iegad {
namespace net {


// 前置声明
class tcp_server;
class tcp_connection;
class tcp_service;


class tcp_event {
// tcpg事件抽象类
public:
    typedef std::shared_ptr<tcp_event> ptr_t;


    // ==============================
    // @析构函数
    // ==============================
    virtual
    ~tcp_event();


    // ==============================
    // @服务端初始化事件, 当tcp_server初始化完成时调用
    // @svr: 初始化完成的tcp_server对象指针
    // ==============================
    virtual void
    on_server_init(void *svr) = 0;


    // ==============================
    // @服务端接收连接错误, 当tcp_server accept出现错误时调用
    // @svr: 出现错误的tcp_server对象指针
    // ==============================
    virtual void
    on_server_accept_error(void *) = 0;


    // ==============================
    // @服务端关闭, 当tcp_server停止服务时调用
    // @svr: 关闭的tcp_server对象指针
    // ==============================
    virtual void
    on_server_shutdown(void *) = 0;


    // ==============================
    // @客户端连接, 当tcp_server接收到连接时调用
    // @c: 完成连接的连接端对象指针
    // ==============================
    virtual void
    on_connected(std::shared_ptr<tcp_connection> c) = 0;


    // ==============================
    // @客户端断开连接, 当tcp_server接收到连接断开时调用
    // @c: 断开连接的连接端对象指针
    // ==============================
    virtual void
    on_disconnected(std::shared_ptr<tcp_connection> c) = 0;


    // ==============================
    // @客户端错误, 当tcp_server接收到连接端错误时调用
    // @c: 发生错误的连接端对象指针
    // ==============================
    virtual void
    on_error(std::shared_ptr<tcp_connection> c) = 0;


    // ==============================
    // @客户端消息, 当tcp_server接收到连接端消息时调用
    // @c: 接收到消息的连接端对象指针
    // ==============================
    virtual void
    on_message(std::shared_ptr<tcp_connection> c, const std::string &msg) = 0;


protected:
    // ==============================
    // @受保护的构造函数,
    //  派生类必需使用工厂函数来创建对象智能指针
    // ==============================
    tcp_event()
    {}
}; // class tcp_event


} // namespace net;
} // namespace iegad;



#endif // __IEGAD_TCP_EVENT__
