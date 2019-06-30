#ifndef IEGAD_TCPSERVICE_HPP
#define IEGAD_TCPSERVICE_HPP


#include <iegad_net.hpp>
#include <iegad_tcpevent.hpp>
#include <iegad_tcpconn.hpp>


namespace iegad {
namespace net {


class tcp_service {
public:
    typedef std::shared_ptr<tcp_service> ptr_t;


    static ptr_t
    create(const char *host, const char *svc, tcp_event::ptr_t tcp_ev)
    {
        return ptr_t(new tcp_service(host, svc, tcp_ev));
    }


    void
    run()
    {
        for (size_t i = 0, n = thread_pool_.size(); i < n; i++) {
            thread_pool_[i].join();
        }
    }


    std::string
    endpoint() const
    {
        return host_ + ":" + svc_;
    }


    int
    listenfd() const
    {
        return listenfd_;
    }


    uint32_t
    nthread() const
    {
        return nthread_;
    }


    bool
    stop_flag() const
    {
        return stop_flag_;
    }


    void
    stop()
    {
        stop_flag_ = true;
    }


private:
    tcp_service(const char *host, const char *svc, tcp_event::ptr_t tcp_ev) :
        host_(host),
        svc_(svc),
        tcp_ev_(tcp_ev),
        listenfd_(tcp::bind_listen(host, svc)),
        nthread_(std::thread::hardware_concurrency() * 2),
        stop_flag_(false)
    {
        ::memset(reserved, 0, sizeof(char) * 7);
        tcp::make_listenfd_addrreuse(listenfd_);

        tcp_ev_->on_server_init(this);

        for (uint32_t i = 0; i < nthread_; i++) {
            thread_pool_.push_back(std::thread(std::bind(&tcp_service::_run, this)));
        }
    }


    void
    _run()
    {
        ssize_t n = 0;

        for (; !stop_flag_;) {

            sockaddr addr;
            socklen_t addr_len = sizeof(addr);
            ::memset(&addr, 0, sizeof(addr));

            int cfd = ::accept(listenfd_, &addr, &addr_len);

            if (cfd < 0) {
                tcp_ev_->on_server_accept_error(this);
                continue;
            }

            tcp::make_socket_non_blocking(cfd);
            tcp_connection::ptr_t c = tcp_connection::create(cfd, &addr, addr_len);
            assert(c);

            tcp_ev_->on_connected(c);

            std::string data;
            n = c->read(&data);

            if (n > 0) {
                tcp_ev_->on_message(c, data);
            }
            else if (n == 0) {
                c->close();
                tcp_ev_->on_disconnected(c);
            }
            else {
                tcp_ev_->on_error(c);
            }

            c->close();
        }
    }


    std::vector<std::thread> thread_pool_;
    std::string host_;
    std::string svc_;
    tcp_event::ptr_t tcp_ev_;
    int listenfd_;
    const uint32_t nthread_;
    bool stop_flag_;
    char reserved[7];
}; // class tcp_service;



} // namespace net
} // namespace iegad



#endif // IEGAD_TCPSERVICE_HPP
