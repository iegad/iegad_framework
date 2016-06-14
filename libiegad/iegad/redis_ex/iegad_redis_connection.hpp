#ifndef __IEGAD_REDIS_CONNECTION__
#define __IEGAD_REDIS_CONNECTION__


#include "iegad_redis_exception.hpp"
#include <hiredis/hiredis.h>
#include <cinttypes>


namespace iegad {
namespace redis {


class connection {
public:
    typedef redisContext * redisContext_ptr;

    explicit connection(const std::string & host, int port = 6379, const std::string & usr = "", const std::string & pwd = "", int timeout = -1)
        : 
    context_(nullptr),
    port_(port), 
    timeout_(timeout),
    usr_(usr), 
    pwd_(pwd),
    host_(host) {
        this->_init();
    }


    void reconnect() {
        this->_release();
        this->_init();
    }


    virtual ~connection() {
        this->_release();
    }


    int Port() const {
        return port_;
    }


    void Port(int val) {
        port_ = val;
    }


    const std::string & Host() const {
        return host_;
    }


    void Host(std::string val) {
        host_ = val;
    }


    int timeout() const {
        return timeout_;
    }


    void timeout(int val) {
        timeout_ = val;
    }


    const std::string & usr() const {
        return usr_;
    }


    void usr(std::string val) {
        usr_ = val;
    }


    const std::string & pwd() const {
        return pwd_;
    }


    void pwd(std::string val) {
        pwd_ = val;
    }


    operator redisContext_ptr &() {
        return this->context_;
    }


private:
    void _init() {
        if (timeout_ > 0) {
            struct timeval tmout = { timeout_, 0 };
            context_ = redisConnectWithTimeout(host_.c_str(), port_, tmout);
            redisSetTimeout(context_, tmout);
        }
        else {
            context_ = redisConnect(host_.c_str(), port_);
        }

        if (context_ == nullptr || context_->err) {
            this->_release();
            throw redis_exception(context_->errstr, context_->err);
        }
    }



    void _release() {
        redisFree(context_);
        context_ = nullptr;
    }

    redisContext * context_;
    int port_;
    int timeout_;
    std::string usr_;
    std::string pwd_;
    std::string host_;
}; // class connection;


} // namespace redis;
} // namespace iegad;


#endif // __IEGAD_REDIS_CONNECTION__;
