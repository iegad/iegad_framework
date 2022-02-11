#ifndef CONN_HPP
#define CONN_HPP


#include <iostream>
#include <memory>
#include <event2/bufferevent.h>
#include <iegad/net/iegad_net.hpp>


namespace iegad {
namespace net {


class sess {
public:
  static void
  release(sess *c) {
    delete c;
  }


  static sess*
  create(event_base *base, evutil_socket_t fd) {
    static bufferevent_data_cb onRead = [](bufferevent *be, void *arg) {
      char buf[4092];
      int n = 0;
      sess *c = (sess *)arg;

      if (c->nleft_ == 0) {
        n = static_cast<int>(bufferevent_read(be, &c->nleft_, sizeof(c->nleft_)));
        assert(n == 4);

        c->nleft_ = from_big_endian(c->nleft_);
        c->nleft_ = ~c->nleft_ ^ head_key();
      }

      while(1) {
        n = static_cast<int>(bufferevent_read(be, buf, sizeof(buf)));
        if (n <= 0) {
          break;
        }

        c->data_.append(std::string(buf, n));
        c->nleft_ -= n;
      }

      if (c->nleft_ == 0) {
        evutil_socket_t cfd = bufferevent_getfd(c->bev_);
        n = writen(cfd, c->data_);
        if (n <= 0) {
            std::cout<<"writen failed...\n";
            return;
        }
        c->data_.clear();
      }
    };


    static bufferevent_event_cb onError = [](bufferevent*, short events, void* arg) {
      if (events & BEV_EVENT_EOF) {
        std::cout << "conn disconnected" << std::endl;
      }
      else if (events & BEV_EVENT_TIMEOUT) {
        std::cout << "conn timeout" << std::endl;
      }
      else {
        std::cout << "conn disconnected error: " << ::evutil_gai_strerror(errno) << std::endl;
      }

      sess* c = (sess*)arg;
      sess::release(c);
    };

    sess *c = new sess();
    c->bev_ = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(c->bev_, onRead, nullptr, onError, c);
    bufferevent_enable(c->bev_, EV_READ|EV_PERSIST|EV_ET);

    return c;
  }


  ~sess()
  {
    if (bev_) {
      bufferevent_free(bev_);
      bev_ = nullptr;
    }
  }


private:
  sess(): nleft_(0), bev_(nullptr) {}


  uint32_t nleft_;
  bufferevent *bev_;
  std::string data_;
}; // class sess


} // namepsace net
} // namespace iegad

#endif // CONN_HPP
