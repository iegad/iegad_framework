#ifndef CONN_HPP
#define CONN_HPP


#include <iostream>
#include <memory>
#include <event2/bufferevent.h>
#include <iegad/net/iegad_net.hpp>
#include <iegad/net/iegad_processor.hpp>


namespace iegad {
namespace net {


class server;


class sess {
public:
  static void
  release(sess *c) 
  {
    delete c;
  }

  static void
  onError(bufferevent*, short events, void* arg)
  {
    std::cout << "---------------------------" << std::endl;

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
  }


  static void onRead(bufferevent* be, void* arg);
  static sess* create(event_base* base, evutil_socket_t fd, server* svr);

  virtual ~sess();


private:
  sess(evutil_socket_t cfd, server *svr): svr_(svr), fd_(cfd), nleft_(0), bev_(nullptr) {}
  
  
  int
  _read()
  {
    char buf[4092];
    int n = 0;

    if (nleft_ == 0) {
      n = static_cast<int>(bufferevent_read(bev_, &nleft_, sizeof(nleft_)));
      assert(n == 4);

      nleft_ = from_big_endian(nleft_);
      nleft_ = ~nleft_ ^ head_key();
    }

    while (1) {
      n = static_cast<int>(bufferevent_read(bev_, buf, sizeof(buf)));
      if (n <= 0) {
        return -1;
      }

      data_.append(std::string(buf, n));
      nleft_ -= n;
    }

    if (nleft_ == 0) {
      return 1;
    }

    return 0;
  }
    
  server *svr_;
  evutil_socket_t fd_;
  uint32_t nleft_;
  bufferevent *bev_;
  std::string data_;
}; // class sess


} // namepsace net
} // namespace iegad

#endif // CONN_HPP
