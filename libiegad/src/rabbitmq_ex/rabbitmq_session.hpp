#ifndef __RABBIT_MQ_SESSION__
#define __RABBIT_MQ_SESSION__



// ============ 说明 ============
//
// @创建日期 : 2016-06-12
// @创建人 : iegad
//
// ============================
// @用途 : rabbimq c++ 扩展, 用来建立会话
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include <string>
#include <cinttypes>
#include <SimpleAmqpClient/SimpleAmqpClient.h>



namespace iegad {
namespace rabbitmq_ex {


struct host_option_t {
// 服务器选项
    // =========================
    // @构造函数
    // @ps : 默认会初始化为localhost最基础信息
    // =========================
    host_option_t() :
        port(5672), host("localhost"), usr("guest"), pwd("guest"), vhost("/")
    {}


    // 端口
    uint32_t port;
    // 服务器ip或名称
    std::string host;
    // 用户名
    std::string usr;
    // 密码
    std::string pwd;
    // 虚拟机
    std::string vhost;
}; // struct host_option_t;


// =========================
// @host_option_t 常量 ： 用作默认参数
// =========================
const host_option_t HOST_OPT;


class session_t {
// 会话
public:
    // =========================
    // @友元类：为了让以下类能访问到channel对象
    // =========================
    friend class queue_t;
    friend class exchange_t;


    // =========================
    // @构造函数
    // @hopt : 服务器选项
    // =========================
    explicit session_t(const host_option_t & hopt = HOST_OPT) :
        ch_(nullptr),
        hopt_(hopt)
    {
        this->open();
    }


    // =========================
    // @用途：打开连接
    // @errstr : 错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    bool open(std::string * errstr = nullptr) {
        try {
            ch_ = AmqpClient::Channel::Create(hopt_.host, hopt_.port, hopt_.usr, hopt_.pwd, hopt_.vhost);
            return true;
        }
        catch (std::exception & ex) {
            if (errstr) {
                *errstr = ex.what();
            }
        }
        return false;
    }


    // =========================
    // @属性：服务器ip或名称
    // =========================
    const std::string & host() const {
        return hopt_.host;
    }


    // =========================
    // @属性： amqp端口
    // =========================
    int port() const {
        return hopt_.port;
    }


    // =========================
    // @属性：用户名
    // =========================
    const std::string & usr() const {
        return hopt_.usr;
    }


    // =========================
    // @属性：密码
    // =========================
    const std::string & pwd() const {
        return hopt_.pwd;
    }


    // =========================
    // @属性：虚拟机
    // =========================
    const std::string & vhost() const {
        return hopt_.vhost;
    }


private:
    // =========================
    // @用途：获取rabbitmq通道
    // @返回值：通道对象.
    // =========================
    AmqpClient::Channel::ptr_t channel() {
        if (ch_ == nullptr) {
            this->open();
        }
        return ch_;
    }

    // 通道对象
    AmqpClient::Channel::ptr_t ch_;
    // 服务器选项信息
    host_option_t hopt_;
}; //class session_t;



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_SESSION__
