#ifndef SERVER_HPP
#define SERVER_HPP


#include <memory.h>
#include <iegad/net/iegad_io_thread.hpp>
#include <iegad/net/iegad_sess.hpp>


namespace iegad {
namespace net {


class server {
public:
  typedef std::shared_ptr<server> ptr_t;


  static ptr_t
  create() {
    return ptr_t(new server);
  }


  void
  run(const char *host, const char *svc) {

    lfd_ = bind_listen(host, svc);
    io_thread::ptr_t w = itp_->get();

    // ---------------------------
    // onAccept: 连接事件
    event_callback_fn onAccept = [](evutil_socket_t lfd, short events, void *arg) {

      static int i = 0;
      if (events & EV_READ) {
        event_base *base = (event_base*)arg;

        evutil_socket_t cfd;
        sockaddr addr;
        socklen_t addrlen = sizeof(addr);
        memset(&addr, 0, sizeof(addr));

        while(1) {
          cfd = ::accept(lfd, &addr, &addrlen);
          if (cfd <= 0) {
            if (errno != EAGAIN) {
              std::cout<<"accept error: "<<evutil_gai_strerror(errno)<<std::endl;
            }
            break;
          }

          sess::create(base, cfd);
          std::cout<<"conntions: "<<++i<<std::endl;
        }
      }
    };

    event *ev = ::event_new(w->base(), lfd_, EV_READ|EV_PERSIST|EV_ET, onAccept, w->base());
    assert(ev);
    assert(!::event_add(ev, nullptr));

    itp_->wait();
    ::event_free(ev);
  }


  void
  stop() {
    ::evutil_closesocket(lfd_);
    itp_->stop();
  }


private:
  server(): lfd_(0), nconn_(0), itp_(io_thread_pool::create(4))
  {}


  evutil_socket_t lfd_;
  std::atomic_int32_t nconn_;
  io_thread_pool::ptr_t itp_;
}; // class server


} // namespace net
} // namespace iegad

#endif // SERVER_HPP
