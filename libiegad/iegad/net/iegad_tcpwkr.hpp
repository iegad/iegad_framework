#ifndef __IEGAD_TCP_WORKER__
#define __IEGAD_TCP_WORKER__


// ============================================
// @说明:
//      Tcp工作线程
//
// @作者: iegad
// @时间: 2019-06-30
// ============================================
//     时间               修改人                 修改说明
// ============================================


#include <iegad_net.hpp>
#include <iegad_tcpconn.hpp>


namespace iegad {
namespace net {


// 前置声明
class tcp_server;


class tcp_worker {
// tcp工作线程
public:
    typedef std::shared_ptr<tcp_worker> ptr_t;
    typedef std::map<int, tcp_connection::ptr_t> conn_map_t;


    // ==============================
    // @工厂函数
    // @svr: 挂载的tcp_server对象指针
    // @PS:
    //      该函数需要判断返回值是否为nullptr
    // ==============================
    static ptr_t
    create(tcp_server *svr)
    {
        return ptr_t(new tcp_worker(svr));
    }


    // ==============================
    // @读回调
    // @cfd: 产生读消息的连接套接字
    // @PS:
    //      框架本身设为连接分片处理,
    //      所以当cfd产生读事件时, cfd一定被挂载到当前工作线程中
    // ==============================
    void
    message_callback(int cfd);


    // ==============================
    // @运行工作线程
    // ==============================
    void
    run()
    {
        std::thread t(std::bind(&tcp_worker::_run, this));
        t.detach();
    }


    // ==============================
    // @当前工作线程中的连接数
    // ==============================
    size_t
    conn_num()
    {
        std::unique_lock<std::mutex> lk(conn_map_mtx_);
        return conn_map_.size();
    }


    // ==============================
    // @当前工作线程ID
    // ==============================
    std::thread::id
    thread_id() const
    {
        return thread_id_;
    }


    // ==============================
    // @添加连接套接字到当前工作线程
    // @cfd: 连接套接字
    // @addr: 连接端地址结构
    // @addr_len: 连接端地址结构长度
    // @返回值: 成功返回0, 否则返回-1
    // @PS:
    //      只有当前工作线程最大连接数以满时才会返回错误
    // ==============================
    int
    add(int cfd, const sockaddr *addr, socklen_t addr_len)
    {
        if (conn_map_.size() >= conn_num()) {
            return -1;
        }

        epoll_event ev;
        ::memset(&ev, 0, sizeof(ev));

        ev.data.fd = cfd;
        ev.events = EPOLLIN | EPOLLET;

        tcp::make_socket_non_blocking(cfd);
        assert(!::epoll_ctl(epfd_, EPOLL_CTL_ADD, cfd, &ev));
        _add_conn(cfd, addr, addr_len);

        return 0;
    }


    // ==============================
    // @移出连接端
    // @cfd: 需要移出的连接套接字
    // @返回值: 永远为0, 否则断言
    // ==============================
    int
    remove(int cfd)
    {
        assert(!::epoll_ctl(epfd_, EPOLL_CTL_DEL, cfd, nullptr));
        _remove_conn(cfd);
        return 0;
    }


    // ==============================
    // @通过套接字查找连接对象指针
    // @cfd: 需要移出的连接套接字
    // @返回值:
    //      套接字绑定的连接对象指针
    // ==============================
    tcp_connection::ptr_t
    find_client(int cfd)
    {
        std::unique_lock<std::mutex> lk(conn_map_mtx_);
        return conn_map_[cfd];
    }


private:
    // ==============================
    // @私有构造函, 实例只能由工厂函数创建
    // @svr: 所挂载的tcp_server对象指针
    // ==============================
    tcp_worker(tcp_server *svr);


    // ==============================
    // @开启epoll事件
    // ==============================
    void
    _run();


    // ==============================
    // @线程安全的 连接表添加操作
    // @cfd: 连接套接字
    // @addr: 连接端地址结构
    // @addr_len: 连接端地址结构长度
    // @PS:
    //      任何对连接表的操作都必需通过线程安全的函数完成
    // ==============================
    void
    _add_conn(int cfd, const sockaddr *addr, socklen_t addr_len)
    {
        std::unique_lock<std::mutex> lk(conn_map_mtx_);
        conn_map_[cfd] = tcp_connection::create(cfd, addr, addr_len);
    }


    // ==============================
    // @线程安全的, 移出连接端
    // @cfd: 需要移出的连接套接字
    // @PS:
    //      任何对连接表的操作都必需通过线程安全的函数完成
    // ==============================
    void
    _remove_conn(int cfd)
    {
        std::unique_lock<std::mutex> lk(conn_map_mtx_);
        auto itr = conn_map_.find(cfd);
        assert(itr != conn_map_.end());
        conn_map_.erase(itr);
    }


    // 连接表锁
    std::mutex conn_map_mtx_;
    // 连接表, 操作该表一定要使用线程安全的函数
    conn_map_t conn_map_;
    // 工作线程ID
    std::thread::id thread_id_;
    // tcp服务挂载点
    tcp_server *svr_;
    // 最大连接数
    uint32_t max_conn_;
    // 工作线程中的epoll描述符
    int epfd_;
    // 对齐字段
    char reserved[8];
}; // class tcp_worker;


} // namespace net
} // namespace iegad


#endif // __IEGAD_TCP_WORKER__
