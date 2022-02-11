#ifndef __EV_WORKER_HPP__
#define __EV_WORKER_HPP__


#include <atomic>
#include <memory>
#include <thread>
#include <vector>
#include <event2/event.h>
#include <assert.h>


namespace iegad {
namespace net {


// ---------------------------------
// io_thread:
//    event_base 工作线程
class io_thread {
public:
  typedef std::shared_ptr<io_thread> ptr_t;


  // ------------------------------------
  // create: 工厂函数
  //    返回工作线程对象指针
  static ptr_t
  create()
  {
    return ptr_t(new io_thread);
  }


  // ------------------------------------
  // run: 启动工作线程
  void
  run()
  {
    t_ = std::thread(&io_thread::_run, this);
  }


  // ------------------------------------
  // stop: 停止工作线程
  void
  stop()
  {
    static char buf[1] = {'A'};
    assert(send(pfd_[1], buf, 1, 0) == 1);
    ::evutil_closesocket(pfd_[1]);
  }


  // ------------------------------------
  // wait: 等待线程退出
  void
  wait()
  {
    t_.join();
  }


  // ------------------------------------
  // base: 获取当前工作线程中的event_base
  event_base*
  base()
  {
    return base_;
  }


private:
  // ------------------------------------
  // 构造函数
  io_thread(): base_(nullptr) {}


  // ------------------------------------
  // 禁用复制构造函数和赋值
  io_thread(const io_thread&);
  io_thread operator=(const io_thread&);


  static void
  on_exit(evutil_socket_t fd, short events, void* arg)
  {
    if (events & EV_READ) {
      event_base* b = (event_base*)arg;
      char buf[1] = { 0 };

      while (1) {
        int n = ::recv(fd, buf, sizeof(buf), 0);
        if (n <= 0) {
          ::evutil_closesocket(fd);
          ::event_base_loopbreak(b);
          break;
        }
      }
    }
  }

  // ------------------------------------
  // _run: 开启event_base轮询
  void
  _run() {
    base_ = event_base_new();
    assert(base_);
    assert(!evutil_socketpair(AF_INET, SOCK_STREAM, 0, pfd_));

    event *ev = event_new(base_, pfd_[0], EV_READ|EV_PERSIST|EV_ET, on_exit, base_);
    assert(ev);
    assert(!event_add(ev, nullptr));

    event_base_dispatch(base_);
    event_free(ev);
    event_base_free(base_);
  }


private:
    event_base *base_;
    evutil_socket_t pfd_[2];
    std::thread t_;
}; // io_thread



// io_thread_pool
class io_thread_pool {
public:
  typedef std::shared_ptr<io_thread_pool> ptr_t;

  static ptr_t
  create(int nsize)
  {
    return ptr_t(new io_thread_pool(nsize));
  }

  void
  wait() 
  {
    for (int i = 0, n = pool_.size(); i < n; i++) {
      pool_[i]->wait();
    }

    pool_.clear();
  }


  void
  stop()
  {
    for (int i = 0, n = pool_.size(); i < n; i++) {
      pool_[i]->stop();
    }
  }


  io_thread::ptr_t
  get()
  {
    return pool_[idx_++ % pool_.size()];
  }


private:
  io_thread_pool(int nsize) : idx_(0)
  {
    for (int i = 0; i < nsize; i++) {
      io_thread::ptr_t it = io_thread::create();
      it->run();
      pool_.push_back(it);
    }
  }


  std::atomic<int> idx_;
  std::vector<io_thread::ptr_t> pool_;
}; // class io_thread_pool


} // namespace net
} // namespace iegad


#endif // __EV_WORKER_HPP__
