#ifndef __REDIS_EX__
#define __REDIS_EX__



#include <hiredis/hiredis.h>
#include <string>



class redispp {
public:
    explicit
    hiredispp(const std::string &host = "127.0.0.1", uint16_t port = 6379, int db = 0) :
        host_(host),
        port_(port),
        db_(db),
        c_(nullptr),
        r_(nullptr)
    {}


    ~hiredispp()
    {
        freeReply();

        if (c_) {
            ::redisFree(c_);
        }
    }


    int
    open(std::string *err, int timeoutsec = 0)
    {
        int n = -1;

        do {
            if (timeoutsec) {
                timeval tmo = {timeoutsec, 0};
                c_ = ::redisConnectWithTimeout(host_.c_str(), port_, tmo);
            }
            else {
                c_ = ::redisConnect(host_.c_str(), port_);
            }

            if (!c_) {
                if (err) *err = "unkown exception";
                break;
            }

            if (c_->err) {
                if (err) *err = c_->errstr;
                break;
            }

            r_ = (redisReply *)::redisCommand(c_, "select %d", db_);

            if (c_->err) {
                if (err) *err = c_->errstr;
                break;
            }

            n = 0;
        } while(0);

        freeReply();

        return  n;
    }


    int
    setex(std::string *res, const std::string &key, const std::string &val, int expr)
    {
        int n = 0;

        freeReply();

        r_ = (redisReply *)::redisCommand(c_, "SETEX %s %d %s", key.c_str(), expr, val.c_str());

        if (!r_ || r_->type != REDIS_REPLY_STATUS) {
            if (c_->err) {
                if (res) *res = c_->errstr;
            }
            else {
                if (res) *res = "unkown error";
            }
            n = -1;
        }
        else {
            if (res) *res = r_->str;
        }

        freeReply();

        return  n;
    }


    int
    get(std::string *res, const std::string &key)
    {
        int n = 0;

        if (!res) {
            return -1;
        }

        freeReply();

        r_ = (redisReply *)::redisCommand(c_, "GET %s", key.c_str());

        if (!r_ || r_->type != REDIS_REPLY_STRING) {
            if (c_->err)
                *res = c_->errstr;
            else
                *res = "unkown error";
            n = -1;
        }
        else {
            *res = r_->str;
        }

        freeReply();

        return  n;
    }


private:
    void
    freeReply()
    {
        if (r_) {
            ::freeReplyObject(r_);
        }
        r_ = nullptr;
    }


    std::string host_;
    uint16_t port_;
    int db_;
    redisContext *c_;
    redisReply *r_;
}; // hiredispp



#endif // __REDIS_EX__
