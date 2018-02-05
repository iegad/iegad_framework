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
        assert(arg);

        tcp_session *sess = (tcp_session *)arg;

        if (ev & EV_READ && ev & EV_ET && sess->sockfd() == sockfd) {
            sess->actime_ = ::time(nullptr);
            sess->svr_->push(sess);
        }
    }


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


    void
    reset(int sockfd)
    {
        _release();
        fd_ = sockfd;
        _init();
    }


    ~tcp_session()
    {
        _release();
    }


    int
    sockfd() const
    {
        return fd_;
    }


    SERVER*
    server()
    {
        return svr_;
    }


    const time_t &
    activeTime() const
    {
        return actime_;
    }


    struct event *
    event()
    {
        return &readEv_;
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
            assert(!event_del(&readEv_));
        }
    }


    void
    _init()
    {
        assert(!evutil_make_socket_nonblocking(fd_));
        assert(!event_assign(&readEv_, svr_->base(), fd_,
                             EV_READ | EV_PERSIST | EV_ET,
                             tcp_session::readHandler, this));
        assert(!event_add(&readEv_, nullptr));
        actime_ = ::time(nullptr);
    }


    int fd_;
    SERVER *svr_;
    time_t actime_;
    struct event readEv_;
}; // class tcp_session;



} // namespace net;
} // namespace iegad;



#endif // __TCP_SESSION__
