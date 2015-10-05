#ifndef __SETUP_SVC__
#define __SETUP_SVC__


#include "net/service_basic.h"


#define SVC_SETUP 11


class setup_svc : public iegad::net::svc_basic {
public:
    explicit setup_svc();
     virtual int action(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr);
};

typedef std::shared_ptr<setup_svc> setup_svc_ptr;


#endif // __SETUP_SVC__