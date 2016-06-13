#ifndef __RABBIT_MQ_PUBLISHER__
#define __RABBIT_MQ_PUBLISHER__



#include "rabbitmq_ex/rabbitmq_sender.hpp"
#include "rabbitmq_ex/rabbitmq_exchange.hpp"



namespace iegad {
namespace rabbitmq_ex {


class publisher : public basic_sender {
public:
    explicit publisher(rabbitmq_ex::exchange_t & exc) :
        basic_sender::basic_sender(exc.channel()),
        exchange_(exc.name())
    {}


    virtual ~publisher() {}


    bool send(const std::string & msgstr, bool persistent = false, std::string * errstr = nullptr) {
        return rabbitmq_ex::basic_sender::send(exchange_, "", msgstr, persistent, false, errstr);
    }


private:
    std::string exchange_;
}; // class producter



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_PUBLISHER__
