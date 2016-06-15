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


            int64_t del(const std::string & __key_, std::string * errstr = nullptr) {
                this->make_cmdstr("DEL ", { __key_ });
                int64_t res;
                return this->action_number(&res, errstr) ? res : -1;
            }


            const std::string dump(const std::string & __key_) {
                this->make_cmdstr("DUMP ", { __key_ });
                std::string res;
                this->action_string(&res);
                return res;
            }


            int64_t exists(const std::string & __key_, std::string * errstr = nullptr) {
                this->make_cmdstr("EXISTS ", { __key_ });
                int64_t res;
                return this->action_number(&res, errstr) ? res : -1;
            }

        
            int64_t expire(const std::string & __key_, int time_in_sec,  std::string * errstr = nullptr) {
                this->make_cmdstr("EXPIRE ", { __key_, std::to_string(time_in_sec) });
                int64_t res;
                return this->action_number(&res, errstr) ? res : -1;
            }


            int64_t expireat(const std::string & __key_, uint64_t unix_timestamp,
                             std::string * errstr = nullptr) {
                this->make_cmdstr("EXPIREAT ", { __key_, std::to_string(unix_timestamp) });
                int64_t res;
                return this->action_number(&res, errstr) ? res : -1;
            }


            int64_t pexpire(const std::string & __key_, uint64_t milliseconds,
                            std::string * errstr = nullptr) {
                this->make_cmdstr("PEXPIRE ", { __key_, std::to_string(milliseconds) });
                int64_t res;
                return this->action_number(&res, errstr) ? res : -1;
            }


            int64_t pexpireat(const std::string & __key_, uint64_t milliseconds,
                              std::string * errstr = nullptr) {
                this->make_cmdstr("PEXPIREAT ", { __key_, " ", std::to_string(milliseconds) });
                int64_t res;
                return this->action_number(&res, errstr) ? res : -1;
            }


            const std::vector<std::string> keys(const std::string & pattern,
                                                std::string * errstr = nullptr) {
                this->make_cmdstr("KEYS ", { pattern });
                std::vector<std::string> res;
                this->action_vector(&res, errstr);
                return res;
            }


            int64_t move(const std::string & __key_, int db,
                         std::string * errstr = nullptr) {
                this->make_cmdstr("MOVE ", { __key_, std::to_string(db) });
                int64_t res;
                return this->action_number(&res, errstr) ? res : -1;
            }


            int64_t persist(const std::string & __key_,
                            std::string * errstr = nullptr) {
                this->make_cmdstr("PERSIST ", { __key_ });
                int64_t res;
                return this->action_number(&res, errstr) ? res : -1;
            }


            int64_t pttl(const std::string & __key_,
                         std::string * errstr = nullptr) {
                this->make_cmdstr("PTTL ", { __key_ });
                int64_t res;
                return this->action_number(&res, errstr) ? res : -1;
            }


            int64_t ttl(const std::string & __key_,
                        std::string * errstr = nullptr) {
                this->make_cmdstr("TTL ", { __key_ });
                int64_t res;
                return this->action_number(&res, errstr) ? res : -1;
            }


            const std::string randomkey() {
                this->make_cmdstr("RANDOMKEY ", {});
                std::string res;
                this->action_string(&res);
                return res;
            }


            bool rename(const std::string & __key_, const std::string & newname,
                        std::string * errstr = nullptr) {
                this->make_cmdstr("RENAME ", { __key_, newname });
                return this->action_status(errstr);
            }


            bool renamenx(const std::string & __key_, const std::string & newname,
                          std::string * errstr = nullptr) {
                this->make_cmdstr("RENAMENX ", { __key_, newname });
                return this->action_status(errstr);
            }


            const std::string type(const std::string & __key_) {
                this->make_cmdstr("TYPE ", { __key_ });
                std::string res;
                this->action_string(&res);
                return res;
            }
        }; // class key_t;


    }
}


#endif // __IEGAD_REDIS_KEY__;
