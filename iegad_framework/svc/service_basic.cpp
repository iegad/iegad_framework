#include "svc/service_basic.h"


iegad::net::svc_basic::svc_basic_ptr 
iegad::net::svc_basic::get_svc(int svc_id, svc_map_t & svc_map)
{
    svc_map_t::iterator itor = svc_map.find(svc_id);
    return itor == svc_map.end() ? nullptr : itor->second;
}


int
iegad::net::svc_basic::regist_svc(const svc_basic_ptr & svc_obj, svc_map_t & svc_map)
{
    svc_map_t::iterator itor = svc_map.find(svc_obj->get_id());
    if (itor == svc_map.end()) {
	svc_map[svc_obj->get_id()] = svc_obj;
	return 0;
    }
    return -1;
}


iegad::net::svc_basic::svc_basic(int svc_id)
    : svc_id_(svc_id) {}


int 
iegad::net::svc_basic::get_id()
{
    return svc_id_;
}


int 
iegad::net::svc_basic::_response(boost::asio::ip::tcp::socket & clnt, const char * rzt, size_t rzt_size)
{
    boost::system::error_code errcode;
    int n = iegad::io::send_n(clnt, rzt, rzt_size, errcode);
    if (errcode) {
	return -1;
    }
    return n;
}
