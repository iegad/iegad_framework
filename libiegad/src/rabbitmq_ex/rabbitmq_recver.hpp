#ifndef __RABBIT_MQ_RECVER__
#define __RABBIT_MQ_RECVER__



#include "rabbitmq_ex/rabbitmq_basic.hpp"
#include <thread>
#include <functional>



namespace iegad {
namespace rabbitmq_ex {



class basic_recver : public rmq_basic {
public:
    typedef AmqpClient::Envelope::ptr_t Envelope_ptr;
    typedef AmqpClient::Channel::ptr_t Channel_ptr;
    typedef std::function<bool(Envelope_ptr, const std::string &)> recvCallbackHandler;


    explicit basic_recver(const std::string & queue,
                          AmqpClient::Channel::ptr_t ch) :
        rmq_basic::rmq_basic(ch),
        stopped_(true),
        rcv_callback_(nullptr),
        consumer_tag_(""),
        queue_(queue)
    {}


    virtual ~basic_recver() {
        this->endRecvMessage();
    }


    bool setQos(int n) {
        if (consumer_tag_ != "") {
            ch_->BasicQos(consumer_tag_, n);
            return true;
        }
        return false;
    }


    virtual bool recv(std::string * msgstr,
                      bool noAck, bool exclusive , int msg_prefetch_count,  int timeout,
                      std::string * errstr = nullptr) {
        try {
            Envelope_ptr env;
            if (consumer_tag_ == "") {
                consumer_tag_ = ch_->BasicConsume(queue_, "", true, noAck,
                                                             exclusive, msg_prefetch_count);
            }
            if (ch_->BasicConsumeMessage(consumer_tag_, env, timeout)) {
                *msgstr = env->Message()->Body();
            }
            return true;
        }
        catch (std::exception & ex) {
            if (errstr) {
                *errstr = ex.what();
            }
        }
        return false;
    }


    virtual bool beginRecvMessage(recvCallbackHandler callback, bool noAck = true) {
        if (!stopped_) {
            return false;
        }
        stopped_ = false;
        if (rcv_callback_ == nullptr && callback == nullptr) {
            return false;
        }

        if (rcv_callback_ == nullptr && callback != nullptr) {
            rcv_callback_ = callback;
        }
        if (consumer_tag_ == "") {
            consumer_tag_= ch_->BasicConsume(queue_, "", true, noAck, false, 1);
        }
        worker_ = std::thread(std::bind(&basic_recver::_processRecvMessage, this, noAck));
        return true;
    }


    virtual void endRecvMessage() {
        stopped_ = true;
        rcv_callback_ = nullptr;
        if (worker_.joinable()) {
            worker_.join();
        }
    }


    const std::string & queue() const {
        return queue_;
    }

    void setQueue(const std::string &queue) {
        queue_ = queue;
    }

    const std::string & consumer_tag() const {
        return consumer_tag_;
    }

    void setConsumer_tag(const std::string &consumer_tag) {
        consumer_tag_ = consumer_tag;
    }


private:
    void _processRecvMessage(bool noAck) {
        std::string errstr;
        Envelope_ptr env;

        while (!stopped_) {
            try {
                if (!ch_->BasicConsumeMessage(consumer_tag_, env, 3)) {
                    continue;
                }
            }
            catch (std::exception & ex) {
                errstr = ex.what();
            }
            if (rcv_callback_ == nullptr) {
                break;
            }
            if (rcv_callback_(env, errstr) && !noAck) {
                ch_->BasicAck(env);
            }
            errstr.clear();
            env = nullptr;
        }
    }


    bool stopped_;
    recvCallbackHandler rcv_callback_;
    std::thread worker_;
    std::string consumer_tag_;
    std::string queue_;
}; // class IPop;



} // namespace rabbitmq_ex;
} // namespace iegad;



#endif // __RABBIT_MQ_RECVER__
