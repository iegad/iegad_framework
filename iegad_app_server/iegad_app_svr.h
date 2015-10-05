#ifndef __IEGAD_APP_SVR__
#define __IEGAD_APP_SVR__


#include <iostream>
#include <string>
#include "iegad_framework.h"
#include "iegad_mysql.h"


using namespace iegad::net;
using namespace iegad::common;
using namespace iegad::string;
using namespace iegad::mysql;



class app_svr : public tcp_mt_svr {
public:
    explicit app_svr(const std::string & host, unsigned int port, char * argv0);
    ~app_svr();

private:
    void _init();
    _LOG log_handler_;
};




#endif // __IEGAD_APP_SVR__