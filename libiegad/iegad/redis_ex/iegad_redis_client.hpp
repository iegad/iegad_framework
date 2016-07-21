#ifndef __IEGAD_REDIS_CLIENT__
#define __IEGAD_REDIS_CLIENT__


#include "iegad_redis_command.hpp"



namespace iegad {
namespace redis {


class client : public command {
public:
    explicit client(connection & conn)
        :
        command(conn) {}


    virtual ~client() {}


    bool auth(const std::string & pwd,
              std::string * errstr = nullptr) {
        this->make_cmdstr("AUTH ", { pwd.c_str() });
        return this->action_status(errstr);
    }


    const std::string echo(const std::string & str) {
        this->make_cmdstr("ECHO ", { str.c_str() });
        std::string res;
        this->action_string(&res);
        return res;
    }


    bool ping(std::string * errstr = nullptr) {
        this->make_cmdstr("PING");
        return this->action_status(errstr);
    }


    bool quit(std::string * errstr = nullptr) {
        this->make_cmdstr("QUIT");
        return this->action_status(errstr);
    }


    bool select(uint32_t index, std::string * errstr = nullptr) {
        this->make_cmdstr("SELECT ", { std::to_string(index).c_str() });
        return this->action_status(errstr);
    }


}; // class server;



} // namespace redis;
} // namespace iegad;


#endif // __IEGAD_REDIS_CLIENT__
