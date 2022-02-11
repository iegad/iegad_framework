#include <iegad/net/iegad_server.hpp>

void iegad::net::server::onAccept(evutil_socket_t lfd, short events, void* arg)
{
  static int i = 0;
  if (events & EV_READ) {
    server* this_ = (server*)arg;
    io_thread::ptr_t w = this_->itp_->get();

    evutil_socket_t cfd;
    sockaddr addr;
    socklen_t addrlen = sizeof(addr);
    memset(&addr, 0, sizeof(addr));

    while (1) {
      cfd = ::accept(lfd, &addr, &addrlen);
      if (cfd <= 0 && errno) {
        if (errno != EAGAIN) {
          std::cout << "accept errcode: " << errno << ", errstr: " << evutil_gai_strerror(errno) << std::endl;
        }
        break;
      }

      sess::create(w->base(), cfd, this_);
      std::cout << "conntions: " << ++i << std::endl;
    }
  }
}