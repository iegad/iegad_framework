#include "up_usrinfo_proxy.h"


up_usrinfo_proxy::up_usrinfo_proxy(const std::string & host, const std::string & svc)
    : iegad::net::proxy_basic<int, up_usrinfo_t, up_usrinfo_msg>(host, svc)
{}

const int 
up_usrinfo_proxy::operator()(const up_usrinfo_t & param)
{
    return this->_up_usrinfo(param);
}


int 
up_usrinfo_proxy::_up_usrinfo(const up_usrinfo_t & param)
{
    if (this->_connect() != 0) {
	return -1;
    }
    std::string rzt;
    msg_.set_log_id(param.log_id);
    msg_.set_nick_name(param.nick_name);
    msg_.set_name(param.name);
    msg_.set_sex(param.sex);
    msg_.set_birth(param.birth);
    msg_.set_hometown(param.hometown);
    msg_.set_location(param.location);
    msg_.set_blood(param.blood);
    msg_.set_tel(param.tel);
    msg_.set_job(param.job);
    msg_.set_school(param.school);

    if (this->_send_msg(12, 0) != 0) {
	return -1;
    }

    if ((rzt = this->_recv()) == "") {
	return -1;
    }
    return iegad::string::to_int32(rzt);
}
