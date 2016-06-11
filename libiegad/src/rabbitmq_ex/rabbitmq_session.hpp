#ifndef __RABBIT_MQ_INTERFACE__
#define __RABBIT_MQ_INTERFACE__



#include <string>
#include <cinttypes>
#include <SimpleAmqpClient/SimpleAmqpClient.h>



namespace iegad {
namespace rabbitmq_ex {


struct host_option_t {
    host_option_t() :
        port(5672), host("localhost"), usr("guest"), pwd("guest"), vhost("/")
    {}

    uint32_t port;
    std::string host;
    std::string usr;
    std::string pwd;
    std::string vhost;
}; // struct host_option_t;


const host_option_t HOST_OPT;


class queue_t;


class session_t {
public:
    friend queue_t;

    explicit session_t(const host_option_t & hopt = HOST_OPT) :
        ch_(nullptr),
        hopt_(hopt)
    {
        this->open();
    }


    bool open(std::string * errstr = nullptr) {
        try {
            ch_ = AmqpClient::Channel::Create(hopt_.host, hopt_.port, hopt_.usr, hopt_.pwd, hopt_.vhost);
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
    AmqpClient::Channel::ptr_t channel() {
        if (ch_ == nullptr) {
            this->open();
        }
        return ch_;
    }

    AmqpClient::Channel::ptr_t ch_;
    host_option_t hopt_;
}; //class session_t;



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_INTERFACE__
