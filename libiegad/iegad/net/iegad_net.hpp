#ifndef __IEGAD_NET_HPP__
#define __IEGAD_NET_HPP__


// ============================================
// @说明:
//      Tcp工具类, 提供对基础操作的封装
//
// @作者: iegad
// @时间: 2019-06-30
// ============================================
//     时间               修改人                 修改说明
// ============================================


// c++头文件
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include <map>
#include <vector>
#include <atomic>

// c头文件
#include <memory.h>
#include <assert.h>

// 系统相关头文件
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>


#include <iostream>


namespace iegad {
namespace net {


class tcp {
// Tcp工具类
public:
    // ==============================
    // @通过host, svc来建立监听套接字
    // @host: ip或域名
    // @svc: 端口或服务
    // @PS:
    //      该函数只会返回成功值. 当失败时会断言
    // ==============================
    static int
    bind_listen(const char *host, const char *svc)
    {
        struct addrinfo hints;
        struct addrinfo *result = nullptr, *rp = nullptr;
        int sfd = -1;

        ::memset(&hints, 0, sizeof (struct addrinfo));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        assert(!::getaddrinfo(host, svc, &hints, &result));

        for(rp = result; rp != nullptr; rp = rp->ai_next) {
            sfd = ::socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

            if (sfd == -1)
                continue;

            if (!::bind(sfd, rp->ai_addr, rp->ai_addrlen))
                break;

            ::close (sfd);
        }

        assert(rp);
        ::freeaddrinfo(result);

        assert(sfd > 0);
        assert(!::listen(sfd, SOMAXCONN));

        return sfd;
    }


    // ==============================
    // @将sfd设置为非阻塞套接字
    // @sfd
    // @PS:
    //      该函数只会成功. 当失败时会断言
    // ==============================
    static void
    make_socket_non_blocking (int sfd)
    {
        int flags = 0;

        flags = ::fcntl(sfd, F_GETFL, 0);
        assert(flags != -1);

        flags |= O_NONBLOCK;
        assert(::fcntl(sfd, F_SETFL, flags) != -1);
    }


    // ==============================
    // @将listenfd设置为监听复用
    // @listenfd
    // @PS:
    //      listenfd必需是监听套接字
    //      该函数只会成功. 当失败时会断言
    // ==============================
    static void
    make_listenfd_addrreuse(int listenfd)
    {
        static const int ON = 1;
        assert(!::setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &ON, sizeof(ON)));
    }


    static void
    set_socket_recv_timeout(int sfd, int s)
    {
        timeval tmout = {s, 0};
        assert(!::setsockopt(sfd, SOL_SOCKET, SO_RCVTIMEO, &tmout, sizeof(tmout)));
    }


private:
    // ==============================
    // @私有构造函数
    // @PS:
    //      该类只提供静态功能函数, 禁止实例化
    // ==============================
    tcp() {}
}; // class tcp;



} // namespace net
} // namespace iegad



#endif // __IEGAD_NET_HPP__
