#ifndef __IEGAD_REDIS_COMMAND__
#define __IEGAD_REDIS_COMMAND__


#include "iegad_redis_connection.hpp"
#include <vector>


namespace iegad {
namespace redis {


    class command {
    protected:
        typedef redisContext * redisContext_ptr;
        typedef redisReply * redisReply_ptr;
        explicit command(connection & conn)
            :
            context_(conn),
            reply_(nullptr)
        {}


        virtual ~command() {
            this->_release();
        }


        bool action() {
            reply_ = (redisReply_ptr)redisCommand(context_, cmd_.c_str());
            this->_assert_handler();
            return reply_->type != REDIS_REPLY_ERROR;
        }


        void action(int64_t * res) {
            reply_ = (redisReply_ptr)redisCommand(context_, cmd_.c_str());
            this->_assert_handler();
            if (this->_assert_type(REDIS_REPLY_INTEGER, "type is not integer"))
                *res = reply_->integer;
        }


        void action(std::string * res) {
            reply_ = (redisReply_ptr)redisCommand(context_, cmd_.c_str());
            this->_assert_handler();
            if (this->_assert_type(REDIS_REPLY_STRING, "type is not string"))
                *res = reply_->str;
        }
 

        void action(std::vector<std::string> * res) {
            reply_ = (redisReply_ptr)redisCommand(context_, cmd_.c_str());
            this->_assert_handler();
            if (this->_assert_type(REDIS_REPLY_ARRAY, "type is not array")) {
                for (int i = 0, n = reply_->elements; i < n; i++) {
                    res->push_back(reply_->element[i]->str);
                }
            }
        }


        void make_cmdstr(const char * arg1, std::initializer_list<const char *> arg2) {
            std::string res(arg1);
            for (auto itor = arg2.begin(); itor != arg2.end(); itor++) {
                res.append(*itor);
                res.append(" ");
            }
            this->cmd_ = res;
        }


    private:
        void _assert_handler() {
            if (reply_ == nullptr || context_->err) {
                throw redis_exception(context_->errstr, context_->err);
            }
        }


        bool _assert_type(int check_type, const std::string & errstr) {
            if (reply_->type != check_type && reply_->type != REDIS_REPLY_NIL) {
                throw redis_exception(errstr.c_str(), reply_->type);
            }
            return reply_->type == check_type;
        }


        void _release() {
            if (reply_ != nullptr) {
                freeReplyObject(reply_);
                reply_ = nullptr;
            }
        }


        redisContext_ptr & context_;
        redisReply_ptr reply_;
        std::string cmd_;

    }; // class redis_command;


} // namespace redis;
} // namespace iegad;


#endif // __IEGAD_REDIS_COMMAND__;
