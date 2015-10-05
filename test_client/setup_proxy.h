#ifndef __SETUP_PROXY__
#define __SETUP_PROXY__


#include "iegad_framework.h"
#include "iegad_app_svr_msg.pb.h"


typedef struct setup_param {
    std::string log_id;
    std::string pwd;
    std::string email;
} setup_t;


class setup_proxy : public iegad::net::proxy_basic<int, setup_t, setup_msg> {
public:
    explicit setup_proxy(const std::string & host, const std::string & svc);
    virtual const int operator()(const setup_t & param);

private:
    int _setup(const setup_t & param);
};


#endif // __SETUP_PROXY__




