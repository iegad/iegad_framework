#ifndef __RABBIT_MQ_SENDER__
#define __RABBIT_MQ_SENDER__



#include "rabbitmq_ex/rabbitmq_basic.hpp"
#include <string>
#include <thread>



namespace iegad {
namespace rabbitmq_ex {



class basic_sender : public rmq_basic {
public:
    explicit basic_sender(AmqpClient::Channel::ptr_t que) :
        rmq_basic::rmq_basic(que)
    {}


    virtual ~basic_sender() {}


    virtual bool send(const std::string & exchange,
                      const std::string & routingKey,
                      const std::string & msgstr, bool persistent, bool mandatory,
                      std::string * errstr = nullptr) {
        try {
            AmqpClient::BasicMessage::ptr_t msg = AmqpClient::BasicMessage::Create(msgstr);
            if (persistent) {
                msg->DeliveryMode(AmqpClient::BasicMessage::delivery_mode_t::dm_persistent);
            }
            ch_->BasicPublish(exchange, routingKey, msg, mandatory);
            return true;
        }
        catch (std::exception & ex) {
            if (errstr) {
                *errstr = ex.what();
            }
        }
        return false;
    }
}; // class IPush;



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_SENDER__
