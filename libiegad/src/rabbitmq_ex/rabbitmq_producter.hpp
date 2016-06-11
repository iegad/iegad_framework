#ifndef __RABBIT_MQ_WORKER__
#define __RABBIT_MQ_WORKER__



#include "rabbitmq_ex/rabbitmq_queue.hpp"
#include <set>



namespace iegad {
namespace rabbitmq_ex {


class producter {
public:
    explicit producter(rabbitmq_ex::queue_t & que) :
        que_(que)
    {}


    bool push(const std::string & msgstr, bool persistent = false, std::string * errstr = nullptr) {
        return rabbitmq_ex::producter::push(que_, msgstr, persistent, errstr);
    }


    static bool push(rabbitmq_ex::queue_t & que,
                     const std::string & msgstr, bool persistent = false, std::string * errstr = nullptr) {
        AmqpClient::BasicMessage::ptr_t msg = AmqpClient::BasicMessage::Create(msgstr);
        if (persistent) {
            msg->DeliveryMode(AmqpClient::BasicMessage::delivery_mode_t::dm_persistent);
        }
        try {
            que.channel()->BasicPublish("", que.name(), msg, true);
            return true;
        }
        catch (std::exception & ex) {
            if (errstr) {
                *errstr = ex.what();
            }
        }
        return false;
    }


private:
    rabbitmq_ex::queue_t & que_;
}; // class producter



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_WORKER__
