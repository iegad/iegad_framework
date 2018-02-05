// ============ 说明 ============
//
// @创建日期 : 2018-02-06
// @创建人 : iegad
//
// ============================
// @用途: 多线程tcp服务端
//
// @PS: 该文件依赖于 libevent 开源库
//
// ============================
//
// @修改记录:
// =======================================
//  日期               修改人                     修改说明
// =======================================



#ifndef __TCP_SERVER__
#define __TCP_SERVER__



#include <event.h>
#include <iegad/net/tcp_session.hpp>
#include <iegad/tools/job_worker.hpp>
#include <thread>
#include <arpa/inet.h>
#include <netinet/tcp.h>



namespace iegad {
namespace net {


enum {
    TE_ERR_ACCEPT,  // 接收客户端连接错误, arg: errno
    TE_ERR_TIMOUT   // 客户端处理事件超时, arg: sockfd
};



class tcp_event {
// tcp服务端事件
public:
    // ============================
    // @用途 : 析构函数
    // ============================
    virtual
    ~tcp_event()
    {}



    // ============================
    // @用途 : 初始化事件, 当服务端启动时触发
    // @返回值 : void
    // ============================
    virtual void
    onInit() = 0;



    // ============================
    // @用途 : 连接事件, 当有客户端连接时触发
    // @sockfd : 客户端套接字
    // @返回值 : void
    // ============================
    virtual void
    onConnected(int sockfd) = 0;



    // ============================
    // @用途 : 处理事件, 当有客户端套接字接收数据前触发
    // @sockfd : 客户端套接字
    // @返回值 : void
    // ============================
    virtual void
    onProcesse(int sockfd) = 0;



    // ============================
    // @用途 : 关闭事件, 当有客户端套接字被关闭前触发
    // @sockfd : 客户端套接字
    // @返回值 : void
    // ============================
    virtual void
    onClosed(int sockfd) = 0;



    // ============================
    // @用途 : 错误事件, 当有错误产生时触发
    // @type : 错误类型
    // @arg : 错误附加参数, 由type决定arg的函意
    // @返回值 : void
    // ============================
    virtual void
    onError(int type, int arg) = 0;
}; // class tcp_event;




// ============================
// @用途 : 处理器
// @PROTOCOL : 协议类型
// @PS : PROTOCOL必需实现readHandler函数,
//       int readHandler(void *arg);
//       arg为tcp_session指针类型
//       成功返回0, 否则返回非0
// ============================
template <typename PROTOCOL>
class tcp_processor {
// 处理器, 用于处理客户端数据
public:
    typedef iegad::tools::job_que_t<void *> que_t;
    typedef iegad::tools::worker_t<void *> work_t;



    // ============================
    // @用途 : 工作线程
    // @sess : 任务数据: tcp_session指针
    // @返回值 : 成功返回0, 否则返回非0
    // ============================
    static int
    workHandler(void *sess)
    {
        return _getProtocol().readHandler(sess);
    }



    // ============================
    // @用途 : 构造函数
    // ============================
    explicit
    tcp_processor() :
        wkr_(que_, tcp_processor::workHandler)
    {
        wkr_.run(1);
    }



    // ============================
    // @用途 : 析构函数
    // ============================
    ~tcp_processor()
    {
        wkr_.stop();
    }



    // ============================
    // @用途 : 将任务投递到任务队列
    // @sess : 任务数据: tcp_session指针
    // @返回值 : void
    // ============================
    void
    push(void *sess)
    {
        que_.push(sess);
    }



    // ============================
    // @用途 : 停止工作
    // @返回值 : void
    // ============================
    void
    stop()
    {
        wkr_.stop();
    }



private:
    // ============================
    // @用途 : 获取当前线程的协议对象
    // @返回值 : 返回协议对象左值引用
    // ============================
    static PROTOCOL &
    _getProtocol()
    {
        static std::map<std::__thread_id, PROTOCOL> tmap;

        std::__thread_id tid = std::this_thread::get_id();

        auto itr = tmap.find(tid);
        if (itr == tmap.end()) {
            PROTOCOL proto;
            tmap.insert(std::make_pair(tid, proto));
        }

        return tmap[tid];
    }


    // 工作队列
    que_t que_;
    // 工作者对象
    work_t wkr_;
}; // class tcp_processor;



// ============================
// @用途 : 多线程tcp服务端
// @PROTOCOL : 协议类型
// @int NTHREAD : 线程数
// @PS : PROTOCOL必需实现readHandler函数,
//       int readHandler(void *arg);
//       arg为tcp_session指针类型
//       成功返回0, 否则返回非0
// ============================
template <typename PROTOCOL, int NTHREAD = 8>
class tcp_server {
// 多线程tcp服务端
public:
    typedef tcp_processor<PROTOCOL> tcp_processor_t;
    typedef tcp_session<tcp_server<PROTOCOL, NTHREAD>> tcp_sess_t;
    typedef tcp_sess_t* tcp_sess_ptr;
    typedef std::map<int, tcp_sess_ptr> tcp_map_t;



    // ============================
    // @用途 : 处理客户端接收事件
    // @fd : 触发接收事件的套接字, 这里为监听套接字
    // @ev : 触发的事件, 这里应该为EV_READ | EV_ET
    // @arg : 事件附加参数, 这里为tcp_server指针
    // @返回值 : void
    // ============================
    static void
    acceptHander(int fd, short ev, void *arg)
    {
        assert(arg);

        tcp_server *svr = (tcp_server *)arg;

        if (fd == svr->listenfd_ &&
            ev & EV_READ &&
            ev & EV_ET) {

            int sockfd = -1;

            for (;;) {
                sockfd = ::accept(fd, nullptr, nullptr);

                if (sockfd > 0) {
                    svr->_acceptHander(sockfd);
                }
                else {
                    if (errno != EAGAIN) {
                        svr->_errorHandler(TE_ERR_ACCEPT, errno);
                    }
                    break;
                }
            } // for (;;)
        }
    }


    explicit
    tcp_server(const std::string &host, int port,
               unsigned int maxConn = 1000, unsigned int expireTime = 0,
               tcp_event *event = nullptr) :
        MAX_CONN(maxConn),
        listenfd_(_tcpBindListen(host, port)),
        curConn_(0),
        expireTime_(expireTime),
        base_(::event_base_new()),
        event_(event)
    {
        _init();
    }


    ~tcp_server()
    {
        for (auto itr = conns_.begin(); itr != conns_.end(); itr++) {
            delete itr->second;
        }
    }


    event_base*
    base()
    {
        return base_;
    }


    void
    push(tcp_sess_ptr sess)
    {
        time_t tnow = ::time(nullptr);
        idx_ = ++idx_ % NTHREAD;

        if (expireTime_ != 0 &&
            tnow - sess->activeTime() > expireTime_) {
            if (event_) {
                event_->onError(TE_ERR_TIMOUT, sess->sockfd());
            }
        }
        else {
            if (event_) {
                event_->onProcesse(sess->sockfd());
            }
            procPool_[idx_].push(sess);
        }
    }


    void
    run()
    {
        assert(!::event_base_dispatch(base_));
    }


    unsigned int
    maxConn() const
    {
        return MAX_CONN;
    }


    unsigned int
    currentConn() const
    {
        return curConn_;
    }


    int
    sockfd() const
    {
        return listenfd_;
    }


    const std::map<size_t, tcp_sess_ptr> &
    conns()
    {
        return conns_;
    }


    void
    closeSession(tcp_sess_ptr sess)
    {
        if (event_) {
            event_->onClosed(sess->sockfd());
        }

        assert(!::evutil_closesocket(sess->sockfd()));
        assert(!::event_del(sess->event()));
        curConn_--;
    }


private:
    static int
    _tcpBindListen(const std::string &host, int port)
    {
        static const int ON = 1;
        static const int BAKLOG = 128;
        static const linger ling = {0, 0};

        int sfd;
        struct sockaddr_in addr;
        ::memset(&addr, 0, sizeof(addr));

        sfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        assert(sfd > 0);

        assert(!::evutil_make_socket_nonblocking(sfd));
        assert(!::setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &ON, sizeof(ON)));
        assert(!::setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, &ON, sizeof(ON)));
        assert(!::setsockopt(sfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)));
        assert(!::setsockopt(sfd, IPPROTO_TCP, TCP_NODELAY, &ON, sizeof(ON)));

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = ::inet_addr(host.c_str());
        addr.sin_port = htons(port);

        assert(!::bind(sfd, (sockaddr *)&addr, sizeof(addr)));
        assert(!::listen(sfd, BAKLOG));

        return sfd;
    }


    void
    _errorHandler(int type, int errcode)
    {
        if (event_) {
            event_->onError(type, errcode);
        }
    }


    void
    _init()
    {
        if (event_) {
            event_->onInit();
        }

        assert(base_);
        assert(!::event_assign(&acceptEv_, base_, listenfd_,
                             EV_READ | EV_PERSIST | EV_ET,
                             tcp_server::acceptHander, this));
        assert(!::event_add(&acceptEv_, nullptr));
    }


    void
    _acceptHander(int sockfd)
    {
        tcp_sess_ptr sess = nullptr;

        if (MAX_CONN > curConn_) {
            auto itr = conns_.find(sockfd);

            if (itr == conns_.end()) {
                sess = new tcp_sess_t(this, sockfd);
                conns_.insert(std::make_pair(sockfd, sess));
            }
            else {
                itr->second->reset(sockfd);
            }

            curConn_++;
        }
        else {
            ::evutil_closesocket(sockfd);
        }
    }


    const unsigned int MAX_CONN;

    int listenfd_;
    unsigned int curConn_;
    unsigned int idx_;
    unsigned int expireTime_;

    event_base *base_;
    tcp_event *event_;
    event acceptEv_;
    tcp_processor_t procPool_[NTHREAD];
    tcp_map_t conns_;
}; // class tcp_server;



} // namespace net;
} // namespace iegad;



#endif // __TCP_SERVER__
