#ifndef __ECHO_SVC__
#define __ECHO_SVC__



#include "nets/basic_svc.h"


enum {
    ECHO_SVC = 10
};



class echo_svc : public iegad::nets::basic_svc {
public:
    explicit echo_svc()
        : iegad::nets::basic_svc(ECHO_SVC) {}


    int action(boost::asio::ip::tcp::socket &clnt,
               int msg_flag,
               const std::string &msg_bdstr) {
        return this->_return(clnt, msg_bdstr.c_str(), msg_bdstr.size());
    }
};



#endif // __ECHO_SVC__

