#ifndef __IEGAD_TCP_SERVER__
#define __IEGAD_TCP_SERVER__


// ============================================
// @说明:
//      Tcp服务,
//      由于Tcp服务使用epoll模型, 所以必需是linux平台下使用
//      该服务最好是使用在长连接场景下,
//      如果是短连接服务请使用tcp_service
//
// @作者: iegad
// @时间: 2019-06-30
// ============================================
//     时间               修改人                 修改说明
// ============================================


#include <iegad_tcpwkr.hpp>
#include <iegad_tcpevent.hpp>


namespace iegad {
namespace net {


class tcp_server {
// tcp服务类
public:
    typedef std::shared_ptr<tcp_server> ptr_t;


    // ==============================
    // @工厂函数
    // @host: ip或域名
    // @svc: 端口或服务
    // @tcp_ev: tcp事件对象指针
    // @per_thread_max_conn_: 当线程中最大连接数
    // @PS:
    //      该函数需要判断返回值是否为nullptr
    // ==============================
    static ptr_t
    create(const char *host, const char *svc, tcp_event::ptr_t tcp_ev, uint32_t per_thread_max_conn_ = 10000)
    {
        assert(tcp_ev);
        return ptr_t(new tcp_server(host, svc, tcp_ev, per_thread_max_conn_));
    }


    // ==============================
    // @析构函数
    // ==============================
    ~tcp_server()
    {
        assert(!::epoll_ctl(epfd_, EPOLL_CTL_DEL, listenfd_, nullptr));
        ::close(listenfd_);
    }


    // ==============================
    // @连接句柄
    // @listenfd: 监听套接字
    // ==============================
    void
    connecte_handler(int listenfd)
    {
        int cfd = -1;
        sockaddr addr;
        socklen_t addr_len = 0;

        for (;;) {
            addr_len = sizeof addr;
            ::memset(&addr, 0, sizeof addr);

            cfd = ::accept(listenfd, &addr, &addr_len);
            if (cfd <= 0) {
                if (errno == EWOULDBLOCK)
                    break;
                else
                    tcp_ev_->on_server_accept_error(this);
            }
            else {
                tcp_worker::ptr_t tw = get_tcpworker();
                assert(tw);
                tw->add(cfd, &addr, addr_len);

                auto c = tw->find_client(cfd);
                assert(c);
                tcp_ev_->on_connected(c);
            }
        }
    }


    // ==============================
    // @断开连接句柄
    // @tw: 工作线程
    // @cfd: 断开连接的套接字
    // ==============================
    void
    disconnect_handler(tcp_worker *tw, int cfd)
    {
        auto c = tw->find_client(cfd);
        assert(c);

        tcp_ev_->on_disconnected(c);
        tw->remove(cfd);
    }


    // ==============================
    // @连接端错误句柄
    // @tw: 工作线程
    // @cfd: 产生错误的连接套接字
    // ==============================
    void
    error_handler(tcp_worker *tw, int cfd)
    {
        auto c = tw->find_client(cfd);
        assert(c);

        tcp_ev_->on_error(c);

        if (tw)
            tw->remove(cfd);
        else
            exit(1);
    }


    // ==============================
    // @连接端消息句柄
    // @c: 产生消息的连接端
    // @msg: 消息
    // ==============================
    void
    message_handler(tcp_connection::ptr_t c, const std::string &msg)
    {
        tcp_ev_->on_message(c, msg);
    }


    // ==============================
    // @获取工作线程
    // @返回值: 返回负载最小的工厂线程对象指针
    // @PS:
    //      为了让每个线程负载均衡, 该函数还是很有必要的,
    //      牺牲了连接性能, 但是大大提高了消息处理的性能
    // ==============================
    tcp_worker::ptr_t
    get_tcpworker()
    {
        size_t idx = 0;
        size_t min = thread_pool_[0]->conn_num();

        for (size_t i = 1, n = thread_pool_.size(); i < n; i++) {
            if (thread_pool_[i]->conn_num() < min) {
                idx = i;
                min = thread_pool_[i]->conn_num();
            }
        }

        return thread_pool_[idx];
    }


    // ==============================
    // @运行服务
    // ==============================
    void
    run()
    {
        int n = 0, i;

        epoll_event *events = reinterpret_cast<epoll_event *>(calloc(1, sizeof(epoll_event)));
        assert(events);

        while(!stop_flag_) {
            n = epoll_wait(epfd_, events, 1, -1);

            for (i = 0; i < n; i++) {
                assert(events[i].data.fd == listenfd_);

                if (events[i].events & EPOLLERR) {
                    continue;
                }

                if (events[i].events & EPOLLHUP) {
                    continue;
                }

                this->connecte_handler(listenfd_);
            }
        }

        ::free(events);
    }


    // ==============================
    // @获取最大连接数
    // ==============================
    uint32_t
    max_conn() const
    {
        return per_thread_max_conn_ * nthread_;
    }


    // ==============================
    // @获取线程数
    // @该线程由框架跟根机器CPU核心数自动计算, 而非人为指定
    // ==============================
    uint32_t
    nthread() const
    {
        return nthread_;
    }


    // ==============================
    // @获取监听套接字
    // ==============================
    int
    listenfd() const
    {
        return listenfd_;
    }


    // ==============================
    // @获取停止标识
    // ==============================
    bool
    stop_flag() const
    {
        return stop_flag_;
    }


    // ==============================
    // @停止服务
    // ==============================
    void
    stop()
    {
        stop_flag_ = true;
    }


    // ==============================
    // @获取监听套接字endpoint字符串形式
    // ==============================
    std::string
    endpoint() const
    {
        return host_ + ":" + svc_;
    }


    // ==============================
    // @获取单线程最大连接数
    // ==============================
    uint32_t
    per_thread_max_conn() const
    {
        return per_thread_max_conn_;
    }


private:
    // ==============================
    // @私有构造函数
    // @host: ip或域名
    // @svc: 端口或服务
    // @tcp_ev: tcp事件对象指针
    // @per_thread_max_conn_: 当线程中最大连接数
    // ==============================
    tcp_server(const char *host, const char *svc, tcp_event::ptr_t tcp_ev, uint32_t per_thread_max_conn) :
        host_(host),
        svc_(svc),
        tcp_ev_(tcp_ev),
        per_thread_max_conn_(per_thread_max_conn),
        nthread_(std::thread::hardware_concurrency() * 2),
        listenfd_(tcp::bind_listen(host, svc)),
        epfd_(-1),
        stop_flag_(false)
    {

        tcp::make_listenfd_addrreuse(listenfd_);
        tcp::make_socket_non_blocking(listenfd_);
        ::memset(reserved_, 0, sizeof(char) * 7);

        epoll_event ev;
        ::memset(&ev, 0, sizeof ev);

        ev.data.fd = listenfd_;
        ev.events = EPOLLIN | EPOLLET;

        epfd_ = epoll_create1(0);
        assert(epfd_ > 0);
        assert(!::epoll_ctl(epfd_, EPOLL_CTL_ADD, listenfd_, &ev));

        for (uint32_t i = 0; i < nthread_; i++) {
            tcp_worker::ptr_t tw = tcp_worker::create(this);
            tw->run();
            thread_pool_.push_back(tw);
        }

        tcp_ev_->on_server_init(this);
    }


    // 线程池
    std::vector<tcp_worker::ptr_t> thread_pool_;
    // ip/域名
    std::string host_;
    // port/服务
    std::string svc_;
    // tcp事件
    tcp_event::ptr_t tcp_ev_;

    // 单线程最大连接数
    const uint32_t per_thread_max_conn_;
    // 最大线程数
    const uint32_t nthread_;

    // 监听套接字
    int listenfd_;
    // epoll描述符
    int epfd_;
    // 服务停止标识
    bool stop_flag_;
    // 对齐字段
    char reserved_[7];
}; // class tcp_server;


} // namespace net;
} // namespace iegad;



#endif // IEGAD_TCPSERVER_HPP
