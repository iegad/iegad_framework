#ifndef __IEGAD_REDIS_COMMAND__
#define __IEGAD_REDIS_COMMAND__


#include "iegad_redis_connection.hpp"
#include "string/iegad_string.hpp"
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


            bool action_status(std::string * errstr) {
                reply_ = (redisReply_ptr)redisCommand(context_, cmd_.c_str());
                return this->_assert_handler(errstr) &&
                        reply_->type == REDIS_REPLY_STATUS &&
                        (std::string(reply_->str) == "OK" || reply_->integer == 1 || (std::string(reply_->str) == "PONG"));
            }


            bool action_number(int64_t * res, std::string * errstr) {
                reply_ = (redisReply_ptr)redisCommand(context_, cmd_.c_str());                
                if (this->_assert_handler(errstr)
                        && this->_assert_type(REDIS_REPLY_INTEGER)) {
                    *res = reply_->integer;
                    return true;
                }
                return false;
            }


            bool action_string(std::string * res) {
                reply_ = (redisReply_ptr)redisCommand(context_, cmd_.c_str());
                if (this->_assert_handler(res) &&
                        this->_assert_type(REDIS_REPLY_STRING)) {
                    *res = reply_->str;
                    return true;
                }
                return false;
            }


            void action_vector(std::vector<std::string> * res, std::string * errstr) {
                reply_ = (redisReply_ptr)redisCommand(context_, cmd_.c_str());
                this->_assert_handler(errstr);
                if (this->_assert_type(REDIS_REPLY_ARRAY)) {
                    for (int i = 0, n = reply_->elements; i < n; i++) {
                        res->push_back(reply_->element[i]->str != nullptr ? reply_->element[i]->str : "");
                    }
                }
            }


            void make_cmdstr(const char * arg1, 
                const std::initializer_list<std::string> & arg2 = {},
                const std::initializer_list<std::string> & arg3 = {}) {
                std::string res(arg1);
                for (auto itor = arg2.begin(); itor != arg2.end(); itor++) {
                    res.append(*itor);
                    res.append(" ");
                }

                for (auto itor = arg3.begin(); itor != arg3.end(); itor++) {
                    res.append(*itor);
                    res.append(" ");
                }

                this->cmd_ = res;
            }


        private:
            bool _assert_handler(std::string * errstr) {
                if (reply_ == nullptr || context_->err) {
                    throw redis_exception(context_->errstr, context_->err);
                }
                if (reply_->type == REDIS_REPLY_ERROR) {
                    if (errstr) {
                        *errstr = reply_->str;
                    }
                    return false;
                }
                return true;
            }


            bool _assert_type(int check_type) {
                if (reply_->type != check_type && reply_->type != REDIS_REPLY_NIL) {
                    throw redis_exception("invalied type", reply_->type);
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
