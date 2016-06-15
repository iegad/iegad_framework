#ifndef __IEGAD_REDIS_STRING__
#define __IEGAD_REDIS_STRING__



#include "iegad_redis_command.hpp"



namespace iegad {
    namespace redis {



        class string_t : public command {
        public:
            explicit string_t(connection & conn)
                :
                command(conn)
            {}


            virtual ~string_t() {}


            bool set(const std::string & __key_, const std::string & __value_) {
                this->make_cmdstr("SET ", { __key_, __value_ });
                return this->action();
            }


            const std::string get(const std::string & __key_) {
                this->make_cmdstr("GET ", { __key_ });
                std::string res;
                this->action(&res);
                return res;
            }


            const std::string getrange(const std::string & __key_, int64_t start, int64_t end) {
                this->make_cmdstr("GETRANGE ", { __key_, std::to_string(start), std::to_string(end) });
                std::string res;
                this->action(&res);
                return res;
            }


            const std::string getset(const std::string __key_, const std::string __value_) {
                this->make_cmdstr("GETSET ", { __key_, __value_ });
                std::string res;
                this->action(&res);
                return res;
            }


            int64_t getbit(const std::string __key_, int64_t offset) {
                this->make_cmdstr("GETBIT ", { __key_, std::to_string(offset) });
                int64_t res;
                this->action(&res);
                return res;
            }


            const std::vector<std::string> mget(const std::initializer_list<std::string> & keys) {
                this->make_cmdstr("MGET ", keys);
                std::vector<std::string> res;
                this->action(&res);
                return res;
            }


            int64_t setbit(const std::string & __key_, int64_t offset, int64_t __value_) {
                this->make_cmdstr("SETBIT ", { __key_, std::to_string(offset), std::to_string(__value_) });
                int64_t res;
                this->action(&res);
                return res;
            }


            bool setex(const std::string & __key_, int64_t sec, const std::string & __value_) {
                this->make_cmdstr("SETEX ", { __key_, std::to_string(sec), __value_ });
                return this->action();
            }


            int64_t setnx(const std::string & __key_, const std::string & __value_) {
                this->make_cmdstr("SETNX ", { __key_, __value_ });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t setrange(const std::string & __key_, int64_t offset, const std::string & __value_) {
                this->make_cmdstr("SETRANGE ", { __key_, std::to_string(offset), __value_ });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t strlen(const std::string & __key_) {
                this->make_cmdstr("STRLEN ", { __key_ });
                int64_t res;
                this->action(&res);
                return res;
            }


            bool mset(const std::initializer_list<std::string> & kvs) {
                this->make_cmdstr("MSET ", kvs);
                return this->action();
            }


            int64_t msetnx(const std::initializer_list<std::string> & kvs) {
                this->make_cmdstr("MSETNX ", kvs);
                int64_t res;
                this->action(&res);
                return res;
            }


            bool psetex(const std::string & __key_, int64_t millisec, const std::string __value_) {
                this->make_cmdstr("PSETEX ", { __key_, std::to_string(millisec), __value_ });
                return this->action();
            }


            const std::string incr(const std::string & __key_) {
                this->make_cmdstr("INCR ", { __key_ });
                std::string res;
                this->action(&res);
                return res;
            }


            const std::string incrby(const std::string & __key_, int64_t incr_amout) {
                this->make_cmdstr("INCRBY ", { __key_, std::to_string(incr_amout) });
                std::string res;
                this->action(&res);
                return res;
            }


            const std::string incrbyfloat(const std::string & __key_, double incr_amout) {
                this->make_cmdstr("INCRBYFLOAT ", { __key_, std::to_string(incr_amout) });
                std::string res;
                this->action(&res);
                return res;
            }


            int64_t decr(const std::string & __key_) {
                this->make_cmdstr("DECR ", { __key_ });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t decrby(const std::string & __key_, int64_t decr_amout) {
                this->make_cmdstr("DECRBY ", { __key_, std::to_string(decr_amout) });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t append(const std::string & __key_, const std::string & __value_) {
                this->make_cmdstr("APPEND ", { __key_, __value_ });
                int64_t res;
                this->action(&res);
                return res;
            }


        }; // class string_t;



    } // namespace redis;
} // namespace iegad;



#endif // __IEGAD_REDIS_STRING__