#ifndef __RABBIT_MQ_BASIC__
#define __RABBIT_MQ_BASIC__



#include "rabbitmq_ex/rabbitmq_queue.hpp"



namespace iegad {
namespace rabbitmq_ex {



class rmq_basic {
protected:
    rmq_basic(AmqpClient::Channel::ptr_t ch)
        : ch_(ch)
    {}

    virtual ~rmq_basic() {}

    AmqpClient::Channel::ptr_t ch_;


private:
    rmq_basic(const rmq_basic &);
    rmq_basic & operator=(const rmq_basic &);
}; // class rmq_basic;



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_BASIC__
