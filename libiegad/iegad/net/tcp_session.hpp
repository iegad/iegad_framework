#ifndef __TCP_SESSION__
#define __TCP_SESSION__



#include <event.h>
#include <memory>
#include <assert.h>



namespace iegad {
namespace net {


template <typename SERVER>
class tcp_session {
public:
    static void
    readHandler(int sockfd, short ev, void *arg)
    {
        tcp_session *sess = (tcp_session *)arg;
        sess->actime_ = time(nullptr);
        if (ev & EV_READ && ev & EV_ET && sess->sockfd() == sockfd) {
            sess->svr_->push(sess);
        }
    }


    tcp_session(SERVER *svr) :
        fd_(-1),
        svr_(svr)
    {}


    void
    reset(int sockfd)
    {
        fd_ = sockfd;
        _release();
        _init();
    }


    ~tcp_session()
    {
        _release();
    }


    int
    sockfd()
    {
        return fd_;
    }


    SERVER *
    server()
    {
        return svr_;
    }


    time_t
    activeTime()
    {
        return actime_;
    }


    struct event *
    event()
    {
        return &event_;
    }


    void
    close()
    {
        _release();
    }



private:
    void
    _release()
    {
        if (fd_ > 0) {
            assert(!evutil_closesocket(fd_));
            assert(!event_del(&event_));
        }
    }


    void
    _init()
    {
        assert(!evutil_make_socket_nonblocking(fd_));
        assert(!event_assign(&event_, svr_->base(), fd_,
                             EV_READ | EV_PERSIST | EV_ET,
                             tcp_session::readHandler, this));
        assert(!event_add(&event_, nullptr));
        actime_ = time(nullptr);
    }


    int fd_;
    SERVER *svr_;
    time_t actime_;
    struct event event_;
}; // class tcp_session;



} // namespace net;
} // namespace iegad;



#endif // __TCP_SESSION__
