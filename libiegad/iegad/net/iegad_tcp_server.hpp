#ifndef __IEGAD_TCP_SERVER__
#define __IEGAD_TCP_SERVER__



#include <inttypes.h>
#include <memory.h>
#include <assert.h>
#include <functional>
#include <memory>
#include <boost/asio.hpp>
#include <event2/event.h>

#include "tools/iegad_endian.hpp"
#include "net/iegad_tcp_event.hpp"
#include <thread>



#define LISTENQ         (15)
#define BUFFSIZE        (1440)



namespace iegad {
namespace net {


class tcp_server {
public:
    static void on_accept(int fd, short ev, void * arg) {
        tcp_event * tcpev = (tcp_event *)arg;
        int sockfd = -1;
        if (arg == nullptr) {
            return;
        }
        sockfd = accept(fd, nullptr, nullptr);
        if (sockfd < 0) {
            return;
        }
        timeval tv;
        tv.tv_sec = 40;
        tv.tv_usec = 0;
        linger li;
        li.l_linger = 0;
        li.l_onoff = 1;
        //evutil_make_socket_nonblocking(sockfd);
        //assert(!setsockopt(sockfd, SOL_SOCKET, TCP_NODELAY, &on, sizeof(on)));
        if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &li, sizeof(li)) != 0) {
            std::cout<<errno<<std::endl;
        }
        assert(!setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)));
        event * read_event = new event;
        event_set(read_event, sockfd, EV_READ | EV_PERSIST, on_read, arg);
        event_base_set(tcpev->base(), read_event);
        event_add(read_event, &tv);
        tcp_session::ptr_t session(new tcp_session(sockfd, read_event));
        tcpev->onConnected(session);
    }


    static void on_read(int sockfd, short ev, void * arg) {
        char buffer[BUFFSIZE] = {0};
        bool eof = false, err = false;
        tcp_event * tcpev = (tcp_event *)arg;
        std::string msgstr;
        int n = 0;
        do {
            n = read(sockfd, buffer, BUFFSIZE);
            if (n < 0) {
                err = true;
                break;
            }
            else if (n == 0) {
                eof = true;
                break;
            }
            else {
                msgstr.append(buffer, n);
                if (n != BUFFSIZE) {
                    break;
                }
            }
        } while(true);
        tcp_session::ptr_t session = tcpev->get_session(sockfd);
        if (eof) {
            tcpev->onReadEof(sockfd);
        }
        if (err) {
            tcpev->onReadErr(sockfd);
        }
        if (!msgstr.empty() && session != nullptr) {
            session->setMsgbuff(msgstr);
            std::thread t(std::bind(&tcp_event::onReadBuff, tcpev, session));
            t.detach();
        }
    }


    explicit tcp_server(uint16_t port,
                        tcp_event * ev)
        :
        port_(port),
        sockfd_(-1),
        base_(nullptr),
        ev_(ev) {
        memset(&accept_event_, 0, sizeof(event));
        memset(&addr_, 0, sizeof(sockaddr_in));
        _init();
    }


    ~tcp_server() {
        if (sockfd_ != -1) {
            close(sockfd_);
        }
    }


    void run() {
        event_base_dispatch(base_);
    }



private:
    void _init() {
        sockfd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        int reuse = 1;
        assert(sockfd_ > 0);
        addr_.sin_addr.s_addr = INADDR_ANY;
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port_);
        assert(!setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)));
        assert(!bind(sockfd_, (sockaddr *)&addr_, sizeof(sockaddr)));
        assert(!listen(sockfd_, LISTENQ));
        base_ = event_base_new();        
        assert(base_);
        ev_->setBase(base_);
        event_set(&accept_event_, sockfd_, EV_READ | EV_PERSIST, on_accept, ev_);
        assert(!event_base_set(base_, &accept_event_));        
        assert(!event_add(&accept_event_, nullptr));
    }


    uint16_t port_;
    int sockfd_;
    event_base * base_;
    event accept_event_;
    sockaddr_in addr_;
    tcp_event * ev_;
}; // class tcp_server;



} // namespace net;
} // namespace iegad;



#endif // __IEGAD_TCP_SERVER__
