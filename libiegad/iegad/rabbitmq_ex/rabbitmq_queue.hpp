#ifndef __RABBIT_MQ_QUEUE__
#define __RABBIT_MQ_QUEUE__



// ============ 说明 ============
//
// @创建日期 : 2016-06-12
// @创建人 : iegad
//
// ============================
// @用途 : rabbimq c++ 扩展, 队列相关类
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include "rabbitmq_ex/rabbitmq_session.hpp"



namespace iegad {
namespace rabbitmq_ex {



class queue_t {
// rabbitmq 队列
public:
    // =========================
    // @用途：创建队列
    // @session : 会话对象
    // @queue：队列名称
    // @passive：队列不存在时， 是否抛出异常
    // @durable：队列是否执久化
    // @exclusive：是否是独享队列
    // @auto_delete：是否自动删除
    // @errstr：错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    static bool createQueue(session_t & session,
                            const std::string & queue,
                            bool passive = false, bool durable = false,
                            bool exclusive = false, bool auto_delete = false, std::string * errstr = nullptr)
    {
        try {
            session.ch_->DeclareQueue(queue, passive, durable, exclusive, auto_delete);
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
    // @用途：删除队列
    // @session : 会话对象
    // @queue：队列名称
    // @unused：队列被使用时不被删除
    // @empty：队列为空是不被删除
    // @errstr：错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    static bool deleteQueue(session_t & session,
                            const std::string & queue,
                            bool unused = false, bool empty = false, std::string * errstr = nullptr)
    {
        try {
            session.channel()->DeleteQueue(queue, unused, empty);
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
    // @用途：清空队列
    // @session : 会话对象
    // @queue：队列名称
    // @errstr：错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    static bool purgeQueue(session_t & session, const std::string & queue, std::string * errstr = nullptr)
    {
        try {
            session.channel()->PurgeQueue(queue);
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
    // @name：队列名称
    // =========================
    explicit queue_t(session_t & session, const std::string & name = "") :
        passive_(false),
        durable_(false),
        exclusive_(false),
        auto_delete_(false),
        name_(name),
        ss_(session)
    {}


    // =========================
    // @用途：创建队列
    // @errstr：错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    bool createQueue(std::string * errstr = nullptr) {
        try {
            name_ = ss_.channel()->DeclareQueue(name_, passive_, durable_, exclusive_, auto_delete_);
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
    // @用途：删除队列
    // @unused：队列被使用时不被删除
    // @empty：队列为空是不被删除
    // @errstr：错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    bool deleteQueue(bool unused = false, bool empty = false, std::string * errstr = nullptr) {
        return queue_t::deleteQueue(ss_, name_, unused, empty, errstr);
    }


    // =========================
    // @用途：清空队列
    // @errstr：错误信息
    // @返回值：成功返回true, 否则返回false.
    // =========================
    bool purgeQueue(std::string * errstr = nullptr) {
        return queue_t::purgeQueue(ss_, name_, errstr);
    }


    // =========================
    // @属性：队列不存在时是否报错
    // =========================
    bool passive() const {
        return passive_;
    }

    void setPassive(bool passive) {
        passive_ = passive;
    }

    // =========================
    // @属性：队列是否执久化
    // =========================
    bool durable() const {
        return durable_;
    }

    void setDurable(bool durable) {
        durable_ = durable;
    }


    // =========================
    // @属性：队列是否独享
    // =========================
    bool exclusive() const  {
        return exclusive_;
    }

    void setExclusive(bool exclusive) {
        exclusive_ = exclusive;
    }


    // =========================
    // @属性：队列是否自动删除
    // =========================
    bool auto_delete() const {
        return auto_delete_;
    }

    void setAuto_delete(bool auto_delete) {
        auto_delete_ = auto_delete;
    }

    // =========================
    // @属性：队列名称
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
    // @属性：通信
    // =========================
    AmqpClient::Channel::ptr_t channel() const {
        return ss_.channel();
    }


private:
    // 队列不存在时， 是否报错
    bool passive_;
    // 是否执久化
    bool durable_;
    // 是否独享
    bool exclusive_;
    // 是否自动删除
    bool auto_delete_;
    // 队列名称
    std::string name_;
    // 当前会话
    session_t & ss_;
}; //class que_option_t;



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_QUEUE__
