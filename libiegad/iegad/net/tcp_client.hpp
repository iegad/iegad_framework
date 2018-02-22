#ifndef __TCP_CLIENT__
#define __TCP_CLIENT__



#include "event2/dns.h"
#include "event2/dns_compat.h"
#include "event2/dns_struct.h"
#include "event2/event.h"
#include "event2/event_compat.h"
#include "event2/event_struct.h"
#include "event2/util.h"

#include <string>
#include <assert.h>


#include <netinet/in.h>
#include <arpa/inet.h>

#include <thread>



namespace iegad {
namespace net {



enum {
    SHUTDOWN_RD,
    SHUTDOWN_WR,
    SHUTDOWN_BOTH
};



enum {
    RUN_BLOCK,
    RUN_NONBLOCK
};



class TcpClient {
public:
    typedef std::function<void(TcpClient*)> readCallBack;


    TcpClient(const std::string &host, const std::string &svc, readCallBack readHandler) :
        sockfd_(0),
        base_(nullptr),
        host_(host),
        svc_(svc),
        readHandler_(readHandler)
    {
        assert(svc.size() > 0 && host.size() > 0 && readHandler);

        ::memset(&event_, 0, sizeof(event_));

        _init();
    }



    ~TcpClient()
    {
        this->close();
    }



    int
    close()
    {
        if (base_) {
            event_base_loopexit(base_, nullptr);
            event_base_free(base_);
            base_ = nullptr;
        }

        if (sockfd_ > 0) {
            ::evutil_closesocket(sockfd_);
            sockfd_ = -1;
        }

        return 0;
    }



    int
    shutdown(int type)
    {
        int ret = ::shutdown(sockfd_, type);

        if (type == SHUTDOWN_BOTH) {
            this->close();
        }

        return ret;
    }



    int
    reset(const std::string &host, const std::string &svc)
    {

        if (this->close() < 0) {
            return -1;
        }

        host_ = host;
        svc_ = svc;

        this->_init();

        return 0;
    }



    std::string
    getEndPoint()
    {
        if (sockfd_ <= 0) {
            return "";
        }

        std::string res;
        sockaddr_in saddr;
        socklen_t addrlen;

        if (::getsockname(sockfd_, (sockaddr *)&saddr, &addrlen)) {
            return "";
        }

        res += inet_ntoa(saddr.sin_addr);
        res += ":" + std::to_string(htons(saddr.sin_port));

        return res;
    }



    std::string
    getPeerEndPoint()
    {
        std::string res = inet_ntoa(saddr_.sin_addr);
        res += ":" + std::to_string(htons(saddr_.sin_port));

        return res;
    }



    int
    sockfd() const
    {
        return sockfd_;
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
            n = ::recv(sockfd_, p, nleft, 0);
            if (n == 0) {
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
            n = ::recv(sockfd_, p, nleft, 0);

            if (n == 0) {
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
            n = ::send(sockfd_, p, nleft, 0);
            if (n > 0) {
                nleft -= n;
                p += n;
            }
            else if (errno == EINTR) {
                continue;
            }
            else {
                return -1;
            }
        }

        return ret - nleft;
    }



private:
    static void
    _readHandler(int fd, short ev, void *arg)
    {
        TcpClient *this_ = (TcpClient *)arg;

        if (fd == this_->sockfd() && ev & EV_READ) {
            this_->readHandler_(this_);
        }
    }



    void
    _init()
    {
        int n;
        addrinfo hints, *res, *ressave;

        ::memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;

        n = ::getaddrinfo(host_.c_str(), svc_.c_str(), &hints, &res);

        assert(!n);

        ressave = res;

        sockfd_ = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        do {
            if (!::connect(sockfd_, res->ai_addr, res->ai_addrlen)) {
                ::memcpy(&saddr_, res->ai_addr, res->ai_addrlen);
                break;
            }
        } while((res = res->ai_next) != nullptr);

        assert(res);

        ::freeaddrinfo(ressave);

        assert(sockfd_ > 0);
        base_ = ::event_base_new();
        assert(base_);

        assert(!::event_assign(
                   &event_, base_, sockfd_, EV_READ | EV_PERSIST | EV_ET, TcpClient::_readHandler, this));
        assert(!::event_add(&event_, nullptr));
        assert(!evutil_make_socket_nonblocking(sockfd_));

        std::thread t(std::bind(::event_base_dispatch, base_));
        t.detach();
    }



    int sockfd_;
    sockaddr_in saddr_;
    event_base *base_;
    event event_;
    std::string host_;
    std::string svc_;
    readCallBack readHandler_;
}; // class TcpClient;



} // namespace net;
} // namespace iegad;



#endif // __TCP_CLIENT__
