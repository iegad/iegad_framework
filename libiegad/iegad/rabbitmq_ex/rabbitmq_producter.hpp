#ifndef __RABBIT_MQ_PRODUCTER__
#define __RABBIT_MQ_PRODUCTER__



#include "rabbitmq_ex/rabbitmq_sender.hpp"
#include "rabbitmq_ex/rabbitmq_session.hpp"



namespace iegad {
namespace rabbitmq_ex {


class producter : public basic_sender {
public:
    explicit producter(rabbitmq_ex::queue_t & que) :
        basic_sender::basic_sender(que.channel()),
        queue_(que.name())
    {}


    virtual ~producter() {}


    bool send(const std::string & msgstr, bool persistent = false, std::string * errstr = nullptr) {
        return basic_sender::send("", queue_, msgstr, persistent, true, errstr);
    }

private:
    std::string queue_;
}; // class producter



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_PRODUCTER__
