#ifndef __UP_USRINFO_PROXY__
#define __UP_USRINFO_PROXY__


#include "iegad_framework.h"
#include "iegad_app_svr_msg.pb.h"


typedef struct {
    std::string log_id;
    std::string nick_name;
    std::string name;
    std::string sex;
    std::string birth;
    std::string hometown;
    std::string location;
    std::string blood;
    std::string tel;
    int job;
    std::string school;
} up_usrinfo_t;

class up_usrinfo_proxy : public iegad::net::proxy_basic < int, up_usrinfo_t, up_usrinfo_msg > {
public:
    explicit up_usrinfo_proxy(const std::string & host, const std::string & svc);
    virtual const int operator()(const up_usrinfo_t & param);

private:
    int _up_usrinfo(const up_usrinfo_t & param);
};



#endif // __UP_USRINFO_PROXY__