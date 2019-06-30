#include <iegad_tcpwkr.hpp>
#include <iegad_tcpserver.hpp>


void
iegad::net::tcp_worker::_run()
{
    this->thread_id_ = std::this_thread::get_id();
    static const int MAX_CONN = static_cast<int>(max_conn_);

    int n = 0, i;
    epoll_event *events = reinterpret_cast<epoll_event *>(calloc(max_conn_, sizeof(epoll_event)));

    while(!svr_->stop_flag()) {
        n = ::epoll_wait(epfd_, events, MAX_CONN, -1);

        for (i = 0; i < n; i++) {
            std::cout<<std::this_thread::get_id()<<" actived..."<<std::endl;
            if (events[i].events & EPOLLHUP) {
                continue;
            }

            if (events[i].events & EPOLLERR) {
                svr_->error_handler(this, events[i].data.fd);
                continue;
            }

            this->message_callback(events[i].data.fd);
        }
    }

    ::free(events);
}


void
iegad::net::tcp_worker::message_callback(int cfd)
{
    tcp_connection::ptr_t c = this->find_client(cfd);
    c->active();

    std::string msg;
    ssize_t n = c->read(&msg);

    if (n == 0) {
        svr_->disconnect_handler(this, cfd);
    }
    else if (n < 0) {
        svr_->error_handler(this, cfd);
    }
    else {
        svr_->message_handler(c, msg);
    }
}


iegad::net::tcp_worker::tcp_worker(tcp_server *svr) :
    svr_(svr),
    max_conn_(svr->per_thread_max_conn()),
    epfd_(-1)
{
    ::memset(reserved, 0, sizeof(char) * 4);
    epfd_ = epoll_create1(0);
    assert(epfd_ > 0);
}
