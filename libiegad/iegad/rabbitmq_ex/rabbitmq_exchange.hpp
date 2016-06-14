#ifndef __RABBIT_MQ_EXCHANGE__
#define __RABBIT_MQ_EXCHANGE__



// ============ 说明 ============
//
// @创建日期 : 2016-06-12
// @创建人 : iegad
//
// ============================
// @用途 : rabbimq c++ 扩展, 交换器相关类
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include "rabbitmq_ex/rabbitmq_session.hpp"
#include <string>



namespace iegad {
namespace rabbitmq_ex {



class exchange_t {
// rabbitmq 交换器
public:
    // =========================
    // @友元类： 为了让这些类可以方问channel对象
    // =========================
    friend class publisher;
    friend class subcriber;


    static const std::string EXCHANGE_TYPE_FANOUT;
    static const std::string EXCHANGE_TYPE_TOPIC;
    static const std::string EXCHANGE_TYPE_DIRECT;


    // =========================
    // @用途：创建交换器
    // @session : 会话对象
    // @queue：交换器名称
    // @passive：交换器不存在时， 是否抛出异常
    // @durable：是否执久化
    // @auto_delete：是否自动删除
    // @errstr：错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    static bool createExchange(session_t & session, const std::string & exchange, const std::string & type,
                            bool passive = false, bool durable = false,
                            bool auto_delete = false, std::string * errstr = nullptr)
    {
        try {
            if (type != EXCHANGE_TYPE_DIRECT &&
                    type != EXCHANGE_TYPE_FANOUT &&
                    type != EXCHANGE_TYPE_TOPIC) {
                if (errstr) {
                    *errstr = "type is invalied";
                }
                return false;
            }
            session.ch_->DeclareExchange(exchange, type, passive, durable, auto_delete);
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
    // @用途：删除交换器
    // @session : 会话对象
    // @queue：交换器名称
    // @unused：交换器被使用时不被删除
    // @errstr：错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    static bool deleteExchange(session_t & session, const std::string & exchange,
                            bool unused = false, std::string * errstr = nullptr)
    {
        try {
            session.channel()->DeleteExchange(exchange, unused);
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
    // @构造函数
    // @session : 会话对象
    // @name：交换器名称
    // =========================
    explicit exchange_t(session_t & session, const std::string & name, const std::string & type) :
        passive_(false),
        durable_(false),
        auto_delete_(false),
        name_(name),
        type_(type),
        ss_(session)
    {}


    // =========================
    // @用途：创建交换器
    // @errstr：错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    bool createExchange(std::string * errstr = nullptr) {
        try {
            ss_.channel()->DeclareExchange(name_, type_, passive_, durable_, auto_delete_);
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
    // @用途：删除交换器
    // @unused：交换器被使用时不被删除
    // @empty：交换器为空是不被删除
    // @errstr：错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    bool deleteExchange(bool unused = false, std::string * errstr = nullptr) {
        return exchange_t::deleteExchange(ss_, name_, unused, errstr);
    }


    // =========================
    // @属性：交换器不存在时是否报错
    // =========================
    bool passive() const {
        return passive_;
    }

    void setPassive(bool passive) {
        passive_ = passive;
    }

    // =========================
    // @属性：是否执久化
    // =========================
    bool durable() const {
        return durable_;
    }

    void setDurable(bool durable) {
        durable_ = durable;
    }


    // =========================
    // @属性：是否自动删除
    // =========================
    bool auto_delete() const {
        return auto_delete_;
    }

    void setAuto_delete(bool auto_delete) {
        auto_delete_ = auto_delete;
    }

    // =========================
    // @属性：交换器名称
    // =========================
    const std::string & name() const {
        return name_;
    }


    void setName(const std::string &name) {
        name_ = name;
    }


    // =========================
    // @属性：会话
    // =========================
    const session_t & session() const {
        return ss_;
    }


    // =========================
    // @属性：交换器类型
    // =========================
    const std::string & type() const {
        return type_;
    }

    void setType(const std::string & type) {
        type_ = type;
    }


    // =========================
    // @属性：通信
    // =========================
    AmqpClient::Channel::ptr_t channel() const {
        return ss_.channel();
    }

private:
    // 交换器不存在时， 是否报错
    bool passive_;
    // 是否执久化
    bool durable_;
    // 是否自动删除
    bool auto_delete_;
    // 交换器名称
    std::string name_;
    // 交换器类型
    std::string type_;
    // 当前会话
    session_t & ss_;
}; //class exchange_t;


const std::string exchange_t::EXCHANGE_TYPE_DIRECT = "direct";
const std::string exchange_t::EXCHANGE_TYPE_TOPIC = "topic";
const std::string exchange_t::EXCHANGE_TYPE_FANOUT = "fanout";



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_EXCHANGE__
