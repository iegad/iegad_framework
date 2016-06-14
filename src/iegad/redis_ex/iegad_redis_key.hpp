#ifndef __IEGAD_REDIS_KEY__
#define __IEGAD_REDIS_KEY__


#include "iegad_redis_command.hpp"


namespace iegad {
    namespace redis {
        

        class key_t : public command {
        public:
            explicit key_t(connection & conn) 
                : 
            command(conn) {}


            virtual ~key_t() {}


            int64_t del(const std::string & __key_) {
                this->make_cmdstr("DEL ", { __key_.c_str() });
                int64_t res;
                this->action(&res);
                return res;
            }


            const std::string dump(const std::string & __key_) {
                this->make_cmdstr("DUMP ", { __key_.c_str() });
                std::string res;
                this->action(&res);
                return res;
            }


            int64_t exists(const std::string & __key_) {
                this->make_cmdstr("EXISTS ", { __key_.c_str() });
                int64_t res;
                this->action(&res);
                return res;
            }

        
            int64_t expire(const std::string & __key_, int time_in_sec) {
                this->make_cmdstr("EXPIRE ", { __key_.c_str(), std::to_string(time_in_sec).c_str() });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t expireat(const std::string & __key_, uint64_t unix_timestamp) {
                this->make_cmdstr("EXPIREAT ", { __key_.c_str(), std::to_string(unix_timestamp).c_str() });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t pexpire(const std::string & __key_, uint64_t milliseconds) {
                this->make_cmdstr("PEXPIRE ", { __key_.c_str(), std::to_string(milliseconds).c_str() });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t pexpireat(const std::string & __key_, uint64_t milliseconds) {
                this->make_cmdstr("PEXPIREAT ", { __key_.c_str(), " ", std::to_string(milliseconds).c_str() });
                int64_t res;
                this->action(&res);
                return res;
            }


            const std::vector<std::string> keys(const std::string & pattern) {
                this->make_cmdstr("KEYS ", { pattern.c_str() });
                std::vector<std::string> res;
                this->action(&res);
                return res;
            }


            int64_t move(const std::string & __key_, int db) {
                this->make_cmdstr("MOVE ", { __key_.c_str(), std::to_string(db).c_str() });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t persist(const std::string & __key_) {
                this->make_cmdstr("PERSIST ", { __key_.c_str() });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t pttl(const std::string & __key_) {
                this->make_cmdstr("PTTL ", { __key_.c_str() });
                int64_t res;
                this->action(&res);
                return res;
            }


            int64_t ttl(const std::string & __key_) {
                this->make_cmdstr("TTL ", { __key_.c_str() });
                int64_t res;
                this->action(&res);
                return res;
            }


            const std::string randomkey() {
                this->make_cmdstr("RANDOMKEY ", {});
                std::string res;
                this->action(&res);
                return res;
            }


            bool rename(const std::string & __key_, const std::string & newname) {
                this->make_cmdstr("RENAME ", { __key_.c_str(), newname.c_str() });
                return this->action();
            }


            bool renamenx(const std::string & __key_, const std::string & newname) {
                this->make_cmdstr("RENAMENX ", { __key_.c_str(), newname.c_str() });
                return this->action();
            }


            const std::string type(const std::string & __key_) {
                this->make_cmdstr("TYPE ", { __key_.c_str() });
                std::string res;
                this->action(&res);
                return res;
            }
        }; // class key_t;


    }
}


#endif // __IEGAD_REDIS_KEY__;
