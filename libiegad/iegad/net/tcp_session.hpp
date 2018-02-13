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
// --2018-02-13      -- iegad               1, 为session添加模板参数, Arg_t
//                                          2, 修改命名规则



#ifndef __TCP_SESSION__
#define __TCP_SESSION__



#include "iegad/net/tcp_macro.h"
#include <string>



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
template <typename TcpServer, typename Arg>
class TcpSession {
// tcp连接会话
public:
    typedef boost::shared_ptr<Arg> Arg_ptr;


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

        TcpSession *sess = (TcpSession *)arg;

        if (ev & EV_READ && ev & EV_ET && sess->sockfd() == sockfd) {
            sess->actime_ = ::time(nullptr);
            sess->svr_->push(sess);
        }
    }



    // ============================
    // @用途 : 构造函数
    // @srv : TcpSession对象指针
    // @sockfd : 客户端连接套接字
    // ============================
    explicit
    TcpSession(TcpServer *svr, int sockfd = -1) :
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
    ~TcpSession()
    {
        release();
    }



    // ============================
    // @用途 : 重置会话对象
    // @sockfd : 客户端连接套接字
    // @返回值 : void
    // ============================
    void
    reset(int sockfd)
    {
        this->release();
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
    TcpServer*
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



    void
    setArg(Arg_ptr arg)
    {
        arg_ = arg;
    }



    Arg_ptr
    getArg()
    {
        return arg_;
    }



    int
    readAll(unsigned char *buff, size_t buffLen)
    {
        assert(buff && buffLen > 0);

        int ret = 0, n, nleft = buffLen;
        unsigned char *p = buff;

        for (;;) {
            n = ::recv(fd_, p, nleft, 0);
            if (n == 0) {
                svr_->sessionErrorHandler(TcpErrorEvent::onEOF, fd_, 0);
                return 0;
            }
            else if (n < 0) {
                if (errno == EAGAIN) {
                    return ret;
                }
                else if (errno == EINTR) {
                    continue;
                }
                else {
                    svr_->sessionErrorHandler(TcpErrorEvent::onRead, fd_, errno);
                    return -1;
                }
            }
            else {
                nleft -= n;
                p += n;
                ret += n;
            }
        }
    }



    int
    read(unsigned char *buff, size_t buffLen)
    {
        assert(buff && buffLen > 0);

        int ret = 0, n, nleft = buffLen;
        unsigned char *p = buff;

        while(nleft > 0) {
            n = ::recv(fd_, p, nleft, 0);

            if (n == 0) {
                svr_->sessionErrorHandler(TcpErrorEvent::onEOF, fd_, 0);
                return 0;
            }
            else if (n < 0) {
                if (errno == EAGAIN) {
                    break;
                }
                else if (errno == EINTR) {
                    continue;
                }
                else {
                    svr_->sessionErrorHandler(TcpErrorEvent::onRead, fd_, errno);
                    return -1;
                }
            }
            else {
                nleft -= n;
                p += n;
                ret += n;
            }
        }

        return ret;
    }



    int
    write(const unsigned char *buff, size_t buffLen)
    {
        assert(buff && buffLen > 0);

        int ret = buffLen, n, nleft = buffLen;
        const unsigned char *p = buff;

        while (nleft > 0) {
            n = ::send(fd_, p, nleft, 0);
            if (n > 0) {
                nleft -= n;
                p += n;
            }
            else if (errno == EINTR) {
                continue;
            }
            else {
                svr_->sessionErrorHandler(TcpErrorEvent::onWrite, fd_, errno);
                return -1;
            }
        }

        return ret - nleft;
    }


    // ============================
    // @用途 : 释放资源
    // @返回值 : void
    // ============================
    void
    release()
    {
        if (fd_ > 0) {
            assert(!::evutil_closesocket(fd_));
            assert(!::event_del(&readEv_));
            fd_ = -1;
        }
    }


    std::string
    getEndPoint()
    {
        std::string endpoint;

        sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);

        ::memset(&addr, 0, sizeof(addr));
        if (::getpeername(fd_, (sockaddr *)&addr, &addrlen)) {
            return "";
        }

        endpoint.append(inet_ntoa(addr.sin_addr));
        endpoint.append(":");
        endpoint.append(std::to_string(htons(addr.sin_port)));

        return endpoint;
    }



private:
    // ============================
    // @用途 : 初始化
    // @返回值 : void
    // ============================
    void
    _init()
    {
        assert(!::event_assign(&readEv_, svr_->base(), fd_,
                             EV_READ | EV_PERSIST | EV_ET,
                             TcpSession::readHandler, this));
        assert(!::event_add(&readEv_, nullptr));
        actime_ = ::time(nullptr);
    }



    // 连接套接字
    int fd_;
    // 服务端对象
    TcpServer* svr_;
    // 最后激活时间
    time_t actime_;
    // 读事件
    struct event readEv_;
    // 附加属性
    Arg_ptr arg_;
}; // class TcpSession;



} // namespace net;
} // namespace iegad;



#endif // __TCP_SESSION__
