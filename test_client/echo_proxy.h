#ifndef __ECHO_PROXY__
#define __ECHO_PROXY__


#include "iegad_framework.h"
#include "echo_msg.pb.h"
#include "svc/proxy_basic.hpp"


using namespace iegad::net;


class echo_proxy : public proxy_basic<std::string, std::string, echo_msg> {
public:

    explicit echo_proxy(const std::string & host, const std::string & svc);
    virtual const std::string operator()(const std::string & param);

private:
    const std::string _echo_svc_proc(const std::string & echo_str);
}; // class echo_proxy;


#endif // __ECHO_PROXY__