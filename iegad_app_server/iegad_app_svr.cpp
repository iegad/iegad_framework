#include "iegad_app_svr.h"
#include "setup_svc.h"


app_svr::~app_svr()
{

}


app_svr::app_svr(const std::string & host, unsigned int port, char * argv0)
    : tcp_mt_svr(host, port), log_handler_(argv0)
{
    _init();
}


void 
app_svr::_init()
{
    setup_svc_ptr setup(new setup_svc);
    this->regist_svc(setup);
}
