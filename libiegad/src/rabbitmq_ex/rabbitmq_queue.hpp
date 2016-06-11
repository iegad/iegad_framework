#ifndef __RABBIT_MQ_QUEUE__
#define __RABBIT_MQ_QUEUE__



#include "rabbitmq_ex/rabbitmq_session.hpp"



namespace iegad {
namespace rabbitmq_ex {


class producter;


class queue_t {
public:
    friend producter;

    explicit queue_t(session_t & session, const std::string & name = "") :
        passive_(false),
        durable_(false),
        exclusive_(true),
        auto_delete_(true),
        name_(name),
        ss_(session)
    {}


    static bool createQueue(session_t & session,
                            const std::string & queue,
                            bool passive = false, bool durable = false,
                            bool exclusive = false, bool auto_delete = false, std::string * errstr = nullptr) {
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


    static bool deleteQueue(session_t & session,
                            const std::string & queue,
                            bool unused = false, bool empty = false, std::string * errstr = nullptr) {
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


    bool deleteQueue(bool unused = false, bool empty = false, std::string * errstr = nullptr) {
        return queue_t::deleteQueue(ss_, name_, unused, empty, errstr);
    }


    bool purgeQueue(std::string * errstr = nullptr) {
        try {
            ss_.channel()->PurgeQueue(name_);
            return true;
        }
        catch (std::exception & ex) {
            if (errstr) {
                *errstr = ex.what();
            }
        }
        return false;
    }


    bool passive() const {
        return passive_;
    }

    void setPassive(bool passive) {
        passive_ = passive;
    }

    bool durable() const {
        return durable_;
    }

    void setDurable(bool durable) {
        durable_ = durable;
    }

    bool exclusive() const  {
        return exclusive_;
    }

    void setExclusive(bool exclusive) {
        exclusive_ = exclusive;
    }

    bool auto_delete() const {
        return auto_delete_;
    }

    void setAuto_delete(bool auto_delete) {
        auto_delete_ = auto_delete;
    }

    const std::string & name() const {
        return name_;
    }

    void setName(const std::string &name) {
        name_ = name;
    }

    const std::string & host() const {
        return ss_.hopt_.host;
    }

    int port() const {
        return ss_.hopt_.port;
    }

    const std::string & usr() const {
        return ss_.hopt_.usr;
    }

    const std::string & pwd() const {
        return ss_.hopt_.pwd;
    }

    const std::string & vhost() const {
        return ss_.hopt_.vhost;
    }


private:
    AmqpClient::Channel::ptr_t channel() const {
        return ss_.channel();
    }


    bool passive_;
    bool durable_;
    bool exclusive_;
    bool auto_delete_;
    std::string name_;
    session_t & ss_;
}; //class que_option_t;



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_QUEUE__
