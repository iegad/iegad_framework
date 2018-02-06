// ============ 说明 ============
//
// @创建日期 : 2018-02-07
// @创建人 : iegad
//
// ============================
// @用途: 多线程tcp连接(会话)对象
//
// @PS: 该文件依赖于 libevent 开源库
//
// ============================
//
// @修改记录:
// =======================================
//  日期               修改人                     修改说明
// =======================================



#ifndef __TCP_SESSION__
#define __TCP_SESSION__



#include <event.h>
#include <memory>
#include <assert.h>



namespace iegad {
namespace net {



// ============================
// @用途 : tcp连接会话
// @SERVER : 服务端模板类
// @PS : 该SERVER对象必需是tcp_server类型,
//       为了只使用hpp方式编写库, 所以
//       不能简单的前置专声明, 需要用模板
//       编程方式.
// ============================
template <typename SERVER>
class tcp_session {
// tcp连接会话
public:
    // ============================
    // @用途 : 读事件句柄
    // @sockfd : 产生读事件的客户端套接字
    // @ev : 触发的事件, 这里为EV_READ | EV_ET
    // @arg : 事件附加参数, 这里为tcp_session对象
    // @返回值 : void
    // ============================
    static void
    readHandler(int sockfd, short ev, void *arg)
    {
        assert(arg);

        tcp_session *sess = (tcp_session *)arg;

        if (ev & EV_READ && ev & EV_ET && sess->sockfd() == sockfd) {
            sess->actime_ = ::time(nullptr);
            sess->svr_->push(sess);
        }
    }



    // ============================
    // @用途 : 构造函数
    // @srv : tcp_server对象指针
    // @sockfd : 客户端连接套接字
    // ============================
    explicit
    tcp_session(SERVER *svr, int sockfd = -1) :
        fd_(sockfd),
        svr_(svr)
    {
        assert(svr);
        if (sockfd > 0) {
            _init();
        }
    }



    // ============================
    // @用途 : 析构函数
    // ============================
    ~tcp_session()
    {
        _release();
    }



    // ============================
    // @用途 : 重置会话对象
    // @sockfd : 客户端连接套接字
    // @返回值 : void
    // ============================
    void
    reset(int sockfd)
    {
        _release();
        fd_ = sockfd;
        _init();
    }



    // ============================
    // @用途 : 获取客户端连接套接字
    // @返回值 : 连接套接字
    // ============================
    int
    sockfd() const
    {
        return fd_;
    }



    // ============================
    // @用途 : 获取服务端对象指针
    // @返回值 : 服务端对象指针
    // ============================
    SERVER*
    server()
    {
        return svr_;
    }



    // ============================
    // @用途 : 获取最后激活时间
    // @返回值 : 最后激活时间
    // ============================
    const time_t &
    activeTime() const
    {
        return actime_;
    }



    // ============================
    // @用途 : 获取读事件指针
    // @返回值 : 读事件指针
    // ============================
    struct event *
    event()
    {
        return &readEv_;
    }



    // ============================
    // @用途 : 关闭连接
    // @返回值 : void
    // ============================
    void
    close()
    {
        svr_->closeSession(this);
    }



private:
    // ============================
    // @用途 : 释放资源
    // @返回值 : void
    // ============================
    void
    _release()
    {
        if (fd_ > 0) {
            assert(!::evutil_closesocket(fd_));
            assert(!::event_del(&readEv_));
        }
    }



    // ============================
    // @用途 : 初始化
    // @返回值 : void
    // ============================
    void
    _init()
    {
        assert(!::evutil_make_socket_nonblocking(fd_));
        assert(!::event_assign(&readEv_, svr_->base(), fd_,
                             EV_READ | EV_PERSIST | EV_ET,
                             tcp_session::readHandler, this));
        assert(!::event_add(&readEv_, nullptr));
        actime_ = ::time(nullptr);
    }


    // 连接套接字
    int fd_;
    // 服务端对象
    SERVER *svr_;
    // 最后激活时间
    time_t actime_;
    // 读事件
    struct event readEv_;
}; // class tcp_session;



} // namespace net;
} // namespace iegad;



#endif // __TCP_SESSION__
