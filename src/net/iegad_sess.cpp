#include <iegad/net/iegad_sess.hpp>
#include <iegad/net/iegad_server.hpp>


void iegad::net::sess::onRead(bufferevent* be, void* arg)
{
  sess* c = (sess*)arg;
  int n = c->_read();
  if (n == 1) {
    auto proc = c->svr_->proc();
    proc->onMessage(c->data_);
  }
}

iegad::net::sess* iegad::net::sess::create(event_base* base, evutil_socket_t fd, server* svr)
{
  sess* c = new sess(fd, svr);
  c->bev_ = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

  bufferevent_setcb(c->bev_, onRead, nullptr, onError, c);
  assert(!bufferevent_enable(c->bev_, EV_READ | EV_PERSIST | EV_ET));

  auto proc = c->svr_->proc();
  proc->onConnected(fd);

  return c;
}

iegad::net::sess::~sess()
{
  if (bev_) {
    bufferevent_free(bev_);
    bev_ = nullptr;
    svr_->proc()->onDisconnected(fd_);
  }
}
