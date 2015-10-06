#ifndef __UP_USRINFO_SVC__
#define __UP_USRINFO_SVC__


#include "svc/service_basic.h"
#include "msg_type.h"


typedef 


class up_usrinfo_svc : public iegad::net::svc_basic {
public:
    explicit up_usrinfo_svc();
    virtual int action(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr);

};

typedef std::shared_ptr<up_usrinfo_svc> up_usrinfo_svc_ptr;

#endif // __UP_USRINFO_SVC__