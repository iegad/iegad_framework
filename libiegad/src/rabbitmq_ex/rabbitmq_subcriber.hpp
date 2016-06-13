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


    explicit subcirber(rabbitmq_ex::exchange_t & exc) :
        rabbitmq_ex::basic_recver::basic_recver("", exc.channel()),
        exchange_(exc.name()) {
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



private:
    void _init() {
        queue_ = ch_->DeclareQueue("");
        ch_->BindQueue(queue_, exchange_);
    }

    std::string exchange_;
    std::string queue_;
}; // class consumer;



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_SUBCRIBER__
