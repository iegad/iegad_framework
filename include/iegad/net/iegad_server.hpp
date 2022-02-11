#ifndef __IEGAD_NET_SERVER__
#define __IEGAD_NET_SERVER__


#include <memory.h>
#include <iegad/net/iegad_io_thread.hpp>
#include <iegad/net/iegad_sess.hpp>
#include <iegad/net/iegad_processor.hpp>


namespace iegad {
namespace net {


class server {
public:
  typedef std::shared_ptr<server> ptr_t;


  static ptr_t
  create(IProcessor *proc, int poolsize = 4) {
    return ptr_t(new server(proc, poolsize));
  }


  IProcessor*
  proc() const { return proc_; }


  static void onAccept(evutil_socket_t lfd, short events, void* arg);


  void
  run(const char *host, const char *svc)
  {
    lfd_ = bind_listen(host, svc);
    io_thread::ptr_t w = itp_->get();

    event *ev = ::event_new(w->base(), lfd_, EV_READ|EV_PERSIST|EV_ET, onAccept, this);
    assert(ev);
    assert(!event_add(ev, nullptr));

    itp_->wait();
    event_free(ev);
  }


  void
  stop() {
    evutil_closesocket(lfd_);
    itp_->stop();
  }


private:
  server(IProcessor *proc, int poolsize): lfd_(-1), proc_(proc), nconn_(0), itp_(io_thread_pool::create(poolsize))
  {}


  evutil_socket_t lfd_;
  IProcessor* proc_;
  std::atomic_int32_t nconn_;
  io_thread_pool::ptr_t itp_;
}; // class server


} // namespace net
} // namespace iegad

#endif // __IEGAD_NET_SERVER__
