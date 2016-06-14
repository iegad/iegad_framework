#ifndef __RABBIT_MQ_CONSUMER__
#define __RABBIT_MQ_CONSUMER__



#include "rabbitmq_ex/rabbitmq_recver.hpp"



namespace iegad {
namespace rabbitmq_ex {



class consumer : public basic_recver {
public:
    typedef basic_recver::Envelope_ptr Envelope_ptr;
    typedef basic_recver::Channel_ptr Channel_ptr;
    typedef basic_recver::recvCallbackHandler recvCallbackHandler;


    explicit consumer(const std::string & queue, rabbitmq_ex::queue_t & que) :
        basic_recver::basic_recver(queue, que.channel())
    {}


    virtual ~consumer() {
        this->endRecvMessage();
    }


    bool recv(std::string * msgstr, bool noAck = true, int timeout = -1,
                      std::string * errstr = nullptr) {
        return basic_recver::recv(msgstr, noAck, false, 1, timeout, errstr);
    }
}; // class consumer;



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_CONSUMER__
