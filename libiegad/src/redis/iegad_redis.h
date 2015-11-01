#ifndef __IEGAD_REDIS__
#define __IEGAD_REDIS__


#include <string>
#include <hiredis.h>
#include <list>



namespace iegad {
namespace redis {


class redis_ctl {
public:
    explicit redis_ctl(const std::string host, int port, int index = 0);
    ~redis_ctl();

    int connect();
    bool is_open() const;

    void close();

    const std::string get_string(const std::string & key);
    //const std::list<std::string> get_list(const std::string & key);
    const std::string exec(const std::string & cmd);

private:
    bool opened_;
    redisContext * conn_;
    int index_;
    int port_;
    std::string host_;
};


} // namespace redis;
} // namespace iegad;



#endif // __IEGAD_REDIS__
