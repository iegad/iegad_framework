#include "iegad_redis.h"
#include "iegad_define.h"


iegad::redis::redis_ctl::redis_ctl(const std::string host, int port, int index)
    : opened_(false), conn_(nullptr), index_(index), port_(port), host_(host)
{}


iegad::redis::redis_ctl::~redis_ctl()
{
    this->close();
}


int
iegad::redis::redis_ctl::connect()
{
    do {
        conn_ = redisConnect(host_.c_str(), port_);
        if (conn_ == nullptr) {
            break;
        }

        redisReply * rr =(redisReply *)redisCommand(conn_, "SELECT %d", index_);
        if (rr == nullptr) {
            break;
        }

        freeReplyObject(rr);
        opened_ = true;
        return 0;
    } while(false);
    this->close();
    return -1;
}


bool
iegad::redis::redis_ctl::is_open() const
{
    return opened_;
}


void
iegad::redis::redis_ctl::close()
{
    if (conn_ != nullptr) {
        redisFree(conn_);
    }
}


const std::string
iegad::redis::redis_ctl::get_string(const std::string &key)
{
    redisReply * rr = (redisReply *)redisCommand(conn_, "GET %s", key.c_str());
    if (rr == nullptr) {
        return "";
    }
    std::string res(rr->str, rr->len);
    freeReplyObject(rr);
    return res;
}


const std::string
iegad::redis::redis_ctl::exec(const std::string &cmd)
{
    redisReply * rr = (redisReply *)redisCommand(conn_, cmd.c_str());
    if (rr == nullptr) {
        return "";
    }
    std::string res(rr->str, rr->len);
    freeReplyObject(rr);
    return res;
}
