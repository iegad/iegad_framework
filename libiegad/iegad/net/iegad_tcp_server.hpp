#ifndef __IEGAD_TCP_SERVER__
#define __IEGAD_TCP_SERVER__



#include <inttypes.h>
#include <memory.h>
#include <assert.h>
#include <functional>
#include <event.h>

#include "tools/iegad_endian.hpp"
#include "iegad_tcp_session.hpp"



#define LISTENQ         (15)
#define BUFFSIZE      (512)



namespace iegad {
namespace net {


class tcp_server {
public:
    typedef std::function<void(std::shared_ptr<tcp_session>)> connect_callback;
    typedef std::function<int(int, const char *, size_t)> read_callback;
    typedef std::function<void(int)> readeof_callback;
    typedef std::function<void(int)> readerror_callback;


    static void on_accept(int fd, short ev, void * arg) {
        int sockfd = -1;
        if (arg == nullptr) {
            std::cout << "arg is null"<<std::endl;
            return;
        }
        socklen_t len = sizeof(sockaddr_in);
        event_base * base = (event_base *)arg;
        sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        sockfd = accept(fd, (sockaddr *)&addr, &len);
        std::cout << "sock : " << sockfd << std::endl;
        if (sockfd < 0) {
            return;
        }
        event * read_event = new event;
        event_set(read_event, sockfd, EV_READ | EV_PERSIST, on_read, read_event);
        event_base_set(base, read_event);
        event_add(read_event, nullptr);
        std::shared_ptr<tcp_session> session(new tcp_session(sockfd));
        conn_handler_(session);
    }


    static void on_read(int sockfd, short ev, void * arg) {
        char buffer[BUFFSIZE] = {0};
        event * read_event = (event *)arg;
        int n;
        n = read(sockfd, buffer, BUFFSIZE);
        if (n < 0) {
            readerr_handler_(sockfd);
            event_del(read_event);
            delete read_event;
            close(sockfd);
        }
        else if (n == 0) {
            readeof_handler_(sockfd);
            event_del(read_event);
            delete read_event;
        }
        else {
            read_handler_(sockfd, buffer, n);
        }
    }

    explicit tcp_server(uint16_t port,
                        connect_callback connhandler,
                        read_callback readhandler,
                        readeof_callback readeofhandler,
                        readerror_callback readerrhandler)
        :
        port_(port),
        sockfd_(-1),
        base_(nullptr) {
        conn_handler_ = connhandler;
        read_handler_ = readhandler;
        readeof_handler_ = readeofhandler;
        readerr_handler_ = readerrhandler;

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
        event_set(&accept_event_, sockfd_, EV_READ | EV_PERSIST, on_accept, base_);
        assert(!event_base_set(base_, &accept_event_));
        assert(!event_add(&accept_event_, nullptr));
    }


    static connect_callback conn_handler_;
    static read_callback read_handler_;
    static readeof_callback readeof_handler_;
    static readerror_callback readerr_handler_;


    uint16_t port_;
    int sockfd_;
    event_base * base_;
    event accept_event_;
    sockaddr_in addr_;
}; // class tcp_server;


tcp_server::connect_callback tcp_server::conn_handler_;
tcp_server::read_callback tcp_server::read_handler_;
tcp_server::readeof_callback tcp_server::readeof_handler_;
tcp_server::readerror_callback tcp_server::readerr_handler_;


} // namespace net;
} // namespace iegad;



#endif // __IEGAD_TCP_SERVER__
