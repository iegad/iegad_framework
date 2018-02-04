#ifndef __TCP_SERVER__
#define __TCP_SERVER__



#include <event.h>
#include <iegad/net/tcp_session.hpp>
#include <iegad/tools/job_worker.hpp>
#include <thread>
#include <arpa/inet.h>
#include <netinet/tcp.h>



namespace iegad {
namespace net {



template <typename PROTOCOL>
class tcp_processor {
public:
    typedef iegad::tools::job_que_t<void *> que_t;
    typedef iegad::tools::worker_t<void *> work_t;


    static PROTOCOL&
    getProtocol()
    {
        static std::map<std::__thread_id, PROTOCOL> instance;

        std::__thread_id tid = std::this_thread::get_id();

        auto itr = instance.find(tid);
        if (itr == instance.end()) {
            PROTOCOL proto;
            instance.insert(std::make_pair(tid, proto));
        }

        return instance[tid];
    }


    static int
    workHandler(void *sess)
    {
        return getProtocol().readHandler(sess);
    }


    explicit
    tcp_processor() :
        wkr_(que_, tcp_processor::workHandler)
    {
        wkr_.run(1);
    }


    ~tcp_processor()
    {
        wkr_.stop();
    }


    void
    push(void *sess)
    {
        que_.push(sess);
    }


private:
    que_t que_;
    work_t wkr_;
}; // class tcp_processor;



template <typename PROTOCOL, int NTHREAD = 8>
class tcp_server {
public:
    typedef tcp_processor<PROTOCOL> tcp_processor_t;
    typedef tcp_session<tcp_server<PROTOCOL, NTHREAD>> tcp_sess_t;
    typedef tcp_sess_t* tcp_sess_ptr;


    static int
    tcpBindListen(const std::string &host, int port)
    {
        static const int ON        = 1;
        static const int BAKLOG    = 128;
        static const linger ling   = {0, 0};

        int sfd;
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));

        sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        assert(sfd > 0);

        evutil_make_socket_nonblocking(sfd);

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(host.c_str());
        addr.sin_port = htons(port);

        assert(!setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &ON, sizeof(ON)));
        assert(!setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &ON, sizeof(ON)));
        assert(!setsockopt(sfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)));
        assert(!setsockopt(sfd, IPPROTO_TCP, TCP_NODELAY, &ON, sizeof(ON)));

        assert(!bind(sfd, (sockaddr *)&addr, sizeof(addr)));
        assert(!listen(sfd, BAKLOG));

        return sfd;
    }


    static void
    acceptHander(int fd, short ev, void *arg)
    {
        tcp_server *svr = (tcp_server *)arg;

        if (fd == svr->lfd_ && ev & EV_READ && ev & EV_ET) {

            int sockfd = -1;

            for (;;) {
                sockfd = ::accept(fd, nullptr, nullptr);
                if (sockfd < 0 && errno == EAGAIN) {
                    break;
                }

                svr->conns_[sockfd]->reset(sockfd);
            }
        }
    }


    tcp_server(const std::string &host, int port, unsigned int maxConn = 1000) :
        maxConn_(maxConn),
        lfd_(tcpBindListen(host, port)),
        base_(event_base_new())
    {
        _init();
    }


    ~tcp_server()
    {
        for (auto itr = conns_.begin(); itr != conns_.end(); itr++) {
            delete itr->second;
        }
    }


    event_base *
    base()
    {
        return base_;
    }


    void
    push(tcp_sess_ptr sess)
    {
        idx_ = ++idx_ % NTHREAD;
        processors_[idx_].push(sess);
    }


    void
    run()
    {
        event_base_dispatch(base_);
    }



private:
    void
    _init()
    {
        assert(base_);
        assert(!event_assign(&event_, base_, lfd_,
                             EV_READ | EV_PERSIST | EV_ET,
                             tcp_server::acceptHander, this));

        for (size_t i = 3; i < maxConn_ + 3; i++) {
            conns_.insert(std::make_pair(i, new tcp_sess_t(this)));
        }
    }


    const unsigned int maxConn_;
    int lfd_;
    unsigned int curConn_;
    unsigned int idx_;

    event_base *base_;
    event event_;
    tcp_processor_t processors_[NTHREAD];
    std::map<size_t, tcp_sess_ptr> conns_;
}; // class tcp_server;



} // namespace net;
} // namespace iegad;



#endif // __TCP_SERVER__
