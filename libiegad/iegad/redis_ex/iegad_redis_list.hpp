#ifndef __IEGAD_REDIS_LIST__
#define __IEGAD_REDIS_LIST__



#include <list>
#include "iegad_redis_command.hpp"



namespace iegad {
    namespace redis {
        


        class list_t : public command {
        public:
            explicit list_t(connection & conn) 
                : 
            command(conn) 
            {}


            virtual ~list_t() {}


            const std::vector<std::string> blpop(const std::initializer_list<std::string> & list_keys, int timeout = 0) {
                this->make_cmdstr("BLPOP ", list_keys, { std::to_string(timeout) });
                std::vector<std::string> res;
                this->action(&res);
                return res;
            }


            const std::vector<std::string> brpop(const std::initializer_list<std::string> & list_keys, int timeout = 0) {
                this->make_cmdstr("BRPOP ", list_keys, { std::to_string(timeout) });
                std::vector<std::string> res;
                this->action(&res);
                return res;
            }

            
            const std::vector<std::string> brpoplpush(const std::string & __list_, const std::string & alist, int timeout = 0) {
                this->make_cmdstr("BRPOPLPUSH ", { __list_, alist, std::to_string(timeout) });
                std::vector<std::string> res;
                this->action(&res);
                return res;
            }


            const std::string lindex(const std::string & __list_, int64_t pos) {
                this->make_cmdstr("LINDEX ", { __list_, std::to_string(pos) });
                std::string res;
                this->action(&res);
                return res;
            }


            int64_t linsert_before(const std::string & __list_, const std::string & exists_val, const std::string & new_val) {
                this->make_cmdstr("LINSERT ", { __list_, "BEFORE", exists_val, new_val });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t llen(const std::string & __list_) {
                this->make_cmdstr("LLEN ", { __list_ });
                int64_t res;
                this->action(&res);
                return res;
            }


            const std::string lpop(const std::string & __list_) {
                this->make_cmdstr("LPOP ", { __list_ });
                std::string res;
                this->action(&res);
                return res;
            }


            int64_t lpush(const std::string & __list_, const std::initializer_list<std::string> & vals) {
                this->make_cmdstr("LPUSH ", vals);
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t lpushx(const std::string & __list_, const std::initializer_list<std::string> & vals) {
                this->make_cmdstr("LPUSHX ", { __list_ }, vals);
                int64_t res;
                this->action(&res);
                return res;
            }


            const std::vector<std::string> lrange(const std::string & __list_, int64_t start, int64_t end) {
                this->make_cmdstr("LRANGE ", { __list_, std::to_string(start), std::to_string(end) });
                std::vector<std::string> res;
                this->action(&res);
                return res;
            }


            int64_t lrem(const std::string & __list_, int64_t count, const std::string & __value_) {
                this->make_cmdstr("LREM ", { __list_, std::to_string(count), __value_ });
                int64_t res;
                this->action(&res);
                return res;
            }


            bool lset(const std::string & __list_, int64_t index, const std::string & __value_) {
                this->make_cmdstr("LSET ", { __list_, std::to_string(index), __value_ });
                return this->action();
            }


            bool ltrim(const std::string & __list_, int64_t start, int64_t end) {
                this->make_cmdstr("LTRIM ", { __list_, std::to_string(start), std::to_string(end) });
                return this->action();
            }


            bool rpop(const std::string & __list_) {
                this->make_cmdstr("RPOP ", { __list_ });
                return this->action();
            }


            const std::string rpoplpush(const std::string & src_list, const std::string dest_list) {
                this->make_cmdstr("RPOPLPUSH ", { src_list, dest_list });
                std::string res;
                this->action(&res);
                return res;
            }


            int64_t rpush(const std::string & __list_, const std::initializer_list<std::string> & values) {
                this->make_cmdstr("RPUSH ", values);
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t rpushx(const std::string & __list_, const std::initializer_list<std::string> & values) {
                this->make_cmdstr("RPUSHX ", { __list_ }, values);
                int64_t res;
                this->action(&res);
                return res;
            }
        }; // class list_t;



    } // namespace redis;
} // namespace iegad;



#endif // __IEGAD_REDIS_LIST__;