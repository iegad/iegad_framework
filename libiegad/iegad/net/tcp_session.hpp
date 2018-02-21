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



#include "iegad/net/tcp_macro.h"
#include <string>



namespace iegad {
namespace net {



// ============================
// @用途 : tcp连接会话
// @TcpServer : 服务端模板类
// @ARG : 附加属性模板参数,
//        附加参数以智能指针的方式成为TcpSession的成员变量.
// @PS : 该TcpServer对象必需是iegad::net::TcpServer类型
//       为了只使用hpp方式编写库, 所以
//       不能简单的前置专声明, 需要用模板
//       编程方式.
// ============================
template <typename TcpServer, typename ARG>
class TcpSession {
// tcp连接会话
public:
    typedef boost::shared_ptr<ARG> ARG_ptr;


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



    // ============================
    // @用途 : 设置附加参数
    // @arg: 附加参数, boost::shared_ptr<ARG>
    // @返回值 : void
    // ============================
    void
    setArg(ARG_ptr arg)
    {
        arg_ = arg;
    }



    // ============================
    // @用途 : 获取附加参数
    // @返回值 : 附加参数, boost::shared_ptr<ARG>
    // ============================
    ARG_ptr
    getArg()
    {
        return arg_;
    }



    // ============================
    // @用途 : 将所有可读数据读到buff缓冲区中
    // @buff : 缓冲区
    // @buffLen : 缓冲区大小
    // @返回值 : 大于0, 为读到的数据总量,
    //          等于0, 对端主动关闭套接字,
    //          小于0, 读操作出现错误.
    // @PS: 在使用该函数时，一定要根据实际
    //      应用来确定一个够用的缓冲区.
    // ============================
    int
    readAll(void *buff, size_t buffLen)
    {
        assert(buff && buffLen > 0);

        int ret = 0, n, nleft = buffLen;
        unsigned char *p = (unsigned char *)buff;

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



    ARG_ptr
    makeArg()
    {
        return ARG_ptr(new ARG);
    }



    // ============================
    // @用途 : 将指定数据读到buff缓冲区中
    // @buff : 缓冲区
    // @buffLen : 指定要读的数据长度, 同时也应该是缓冲区的大小.
    // @返回值 : 大于0, 为读到的数据总量,
    //          等于0, 对端主动关闭套接字,
    //          小于0, 读操作出现错误.
    // ============================
    int
    read(void *buff, size_t buffLen)
    {
        assert(buff && buffLen > 0);

        int ret = 0, n, nleft = buffLen;
        unsigned char *p = (unsigned char *)buff;

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



    // ============================
    // @用途 : 将指定缓冲中的数据写到对端
    // @buff : 写缓冲区
    // @buffLen : 写缓冲区的大小.
    // @返回值 : 大于0, 为写出的数据总量,
    //          小于0, 写操作出现错误.
    // ============================
    int
    write(const void *buff, size_t buffLen)
    {
        assert(buff && buffLen > 0);

        int ret = buffLen, n, nleft = buffLen;
        const unsigned char *p = (const unsigned char *)buff;

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
            arg_.reset();
            assert(!::evutil_closesocket(fd_));
            assert(!::event_del(&readEv_));
            fd_ = -1;
        }
    }



    // ============================
    // @用途 : 获取对端端点.
    // @返回值 : 对端端点的字符串形式: 192.168.1.111:45666
    // ============================
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
    ARG_ptr arg_;
}; // class TcpSession;



} // namespace net;
} // namespace iegad;



#endif // __TCP_SESSION__
