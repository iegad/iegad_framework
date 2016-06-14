#ifndef __RABBIT_MQ_PUBLISHER__
#define __RABBIT_MQ_PUBLISHER__



#include "rabbitmq_ex/rabbitmq_sender.hpp"
#include "rabbitmq_ex/rabbitmq_exchange.hpp"



namespace iegad {
namespace rabbitmq_ex {


class publisher : public basic_sender {
public:
    explicit publisher(rabbitmq_ex::exchange_t & exc, const std::string & routingKey = "") :
        basic_sender::basic_sender(exc.channel()),
        persistent_(false),
        routingKey_(routingKey),
        exchange_(exc)
    {}


    virtual ~publisher() {}


    bool send(const std::string & msgstr, std::string * errstr = nullptr) {
        return rabbitmq_ex::basic_sender::send(exchange_.name(), routingKey_, msgstr, persistent_, false, errstr);
    }


    bool persistent() const {
        return persistent_;
    }


    void setPersistent(bool persistent) {
        persistent_ = persistent;
    }


    exchange_t & exchange() const {
        return exchange_;
    }

    const std::string & routingKey() const {
        return routingKey_;
    }

    void setRoutingKey(const std::string &routingKey) {
        routingKey_ = routingKey;
    }


private:
    bool persistent_;
    std::string routingKey_;
    exchange_t & exchange_;
};


 // class producter



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_PUBLISHER__
