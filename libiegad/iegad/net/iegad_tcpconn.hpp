#ifndef __IEGAD_TCP_CONNECTION__
#define __IEGAD_TCP_CONNECTION__



// ============================================
// @说明:
//      Tcp连接类
//
// @作者: iegad
// @时间: 2019-06-30
// ============================================
//     时间               修改人                 修改说明
// ============================================



#include <iegad_net.hpp>



namespace iegad {
namespace net {


class tcp_connection {
// tcp连接类
public:
    typedef std::shared_ptr<tcp_connection> ptr_t;


    // ==============================
    // @工厂函数, 创建tcp连接类的对象指针
    // @cfd: 套接字
    // @addr: 连接端地址结构
    // @addr_len: 连接端地址结构长度
    // @返回值: 成功返回连接对象指针,
    //         一般无失败场景, 但仍需判断返回值是否为空
    // ==============================
    static ptr_t
    create(int cfd, const sockaddr *addr, socklen_t addr_len)
    {
        return ptr_t(new tcp_connection(cfd, addr, addr_len));
    }


    // ==============================
    // @将sfd设置为非阻塞套接字
    // @sfd
    // @PS:
    //      该函数只会成功. 当失败时会断言
    // ==============================
    ~tcp_connection()
    {
        this->close();
    }


    // ==============================
    // @读消息
    // @data: 输出参数, 将读到的数据放到data中
    // @返回值: 成功返回读到字节的大小, 连接断开返回0, 错误返回-1
    // ==============================
    ssize_t
    read(std::string *data)
    {
        static const size_t MAX_BUFF_SIZE = 131072;
        static const size_t BUFF_SIZE = 1300;

        char buff[BUFF_SIZE] = {0};
        ssize_t n = 0;

        for (;;) {
            n = ::recv(cfd_, buff, BUFF_SIZE, 0);
            if (n < 0) {
                if (errno == EWOULDBLOCK) {
                    break;
                }
                return -1;
            }
            else if (n == 0) {
                return 0;
            }
            else {
                if (data->size() > MAX_BUFF_SIZE) {
                    return -2;
                }
                data->insert(data->end(), buff, buff + n);
            }
        }

        return static_cast<ssize_t>(data->size());
    }


    // ==============================
    // @写消息
    // @data: 需要写出的数据
    // @返回值: 成功返回写入的数据长度, 否则返回-1
    // @PS: 是否发送完整数据需要判断data.size()是否等于返回值,
    //      相等的时候才算是真正的成功.
    // ==============================
    ssize_t
    write(const std::string &data)
    {
        ssize_t n = 0;
        size_t nleft = data.size();
        const char *p = data.c_str();

        while(nleft > 0) {
            n = ::send(cfd_, p, nleft, 0);

            if (n <= 0) {
                return -1;
            }
            else {
                nleft -= static_cast<size_t>(n);
                p += n;
            }
        }

        return static_cast<ssize_t>(data.size() - nleft);
    }


    // ==============================
    // @激活, 当连接端有数据发送过来时, 记录当前时间
    // @PS: 主要是为长连接时判断超时
    // ==============================
    void
    active()
    {
        active_tm_ = ::time(nullptr);
    }


    // ==============================
    // @获取最后激活时间
    // @返回值: 最后激活时间
    // ==============================
    time_t
    active_time() const
    {
        return active_tm_;
    }


    // ==============================
    // @关闭连接
    // ==============================
    void
    close()
    {
        ::close(cfd_);
    }


    // ==============================
    // @获取连接端的endpoint
    // @返回值: 客户端endpoint字符串形式: => 192.168.1.100:54321
    // ==============================
    std::string
    endpoint()
    {
        std::string ep;

        do {
            char buff[128] = {0};

            if (addr_.sa_family == AF_INET) {
                sockaddr_in *ai = reinterpret_cast<sockaddr_in *>(&addr_);
                ep += ::inet_ntop(ai->sin_family, &ai->sin_addr, buff, 128);
                ep += ":" + std::to_string(htons(ai->sin_port));
            }
            else if (addr_.sa_family == AF_INET6) {
                sockaddr_in6 *ai6 = reinterpret_cast<sockaddr_in6 *>(&addr_);
                ep += ::inet_ntop(ai6->sin6_family, &ai6->sin6_addr, buff, 128);
                ep += ":" + std::to_string(htons(ai6->sin6_port));
            }
        } while(0);

        return ep;
    }


private:
    // ==============================
    // @私有构造函数, 只允许通过工厂函数创建对象
    // @cfd: 连接套接字
    // @addr: 连接端地址结构
    // @addr_len: 连接端地址结构长度
    // ==============================
    tcp_connection(int cfd, const sockaddr *addr, socklen_t addr_len) :
        cfd_(cfd)
    {
        ::memcpy(&addr_, addr, addr_len);
        ::memset(reserved, 0, sizeof(char) * 4);
    }


    // 连接端地址结构
    sockaddr addr_;
    // 最后激活时间
    time_t active_tm_;
    // 连接套接字
    int cfd_;
    // 对齐字段
    char reserved[4];
}; // class tcp_client;



} // namespace net
} // namespace iegad



#endif // __IEGAD_TCP_CONNECTION__
