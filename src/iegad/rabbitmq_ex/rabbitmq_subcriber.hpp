#ifndef __RABBIT_MQ_SUBCRIBER__
#define __RABBIT_MQ_SUBCRIBER__



#include "rabbitmq_ex/rabbitmq_recver.hpp"
#include "rabbitmq_exchange.hpp"



namespace iegad {
namespace rabbitmq_ex {



class subcirber : public basic_recver {
public:
    typedef basic_recver::Envelope_ptr Envelope_ptr;
    typedef basic_recver::Channel_ptr Channel_ptr;
    typedef basic_recver::recvCallbackHandler recvCallbackHandler;


    explicit subcirber(rabbitmq_ex::exchange_t & exc, const std::string & queue = "", const std::string & routingKey = "")
        :
        rabbitmq_ex::basic_recver::basic_recver("", exc.channel()),
        exchange_(exc.name()),
        queue_(queue),
        routingKey_(routingKey) {
        _init();
    }


    virtual ~subcirber() {
        this->endRecvMessage();
    }


    bool recv(std::string * msgstr, int timeout = -1, std::string * errstr = nullptr) {
        if (consumer_tag() == "") {
            setConsumer_tag(ch_->BasicConsume(queue_));
        }
        return basic_recver::recv(msgstr, true, true, 1, timeout, errstr);
        return false;
    }


    const std::string & exchange() const {
        return exchange_;
    }

    void setExchange(const std::string &exchange) {
        exchange_ = exchange;
    }

    const std::string & queue() const {
        return queue_;
    }

    void setQueue(const std::string &queue) {
        queue_ = queue;
    }

    const std::string & routingKey() const {
        return routingKey_;
    }

    void setRoutingKey(const std::string &routingKey) {
        routingKey_ = routingKey;
    }

private:
    void _init() {
        queue_ = ch_->DeclareQueue(queue_);
        ch_->BindQueue(queue_, exchange_, routingKey_);
    }


    std::string exchange_;
    std::string queue_;
    std::string routingKey_;
}; // class consumer;







} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_SUBCRIBER__
