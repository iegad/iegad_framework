#include "svc/basic_svc.h"
#include "string/iegad_string.h"



iegad::net::basic_svc::basic_svc_ptr 
iegad::net::basic_svc::get_svc(int svc_id, svc_map_t & svc_map)
{
    svc_map_t::iterator itor = svc_map.find(svc_id);
    return itor == svc_map.end() ? nullptr : itor->second;
}


int
iegad::net::basic_svc::regist_svc(const basic_svc_ptr & svc_obj, svc_map_t & svc_map)
{
    svc_map_t::iterator itor = svc_map.find(svc_obj->get_id());
    if (itor == svc_map.end()) {
	svc_map[svc_obj->get_id()] = svc_obj;
	return 0;
    }
    return -1;
}


iegad::net::basic_svc::basic_svc(int svc_id)
    : svc_id_(svc_id) {}


int 
iegad::net::basic_svc::get_id()
{
    return svc_id_;
}


int 
iegad::net::basic_svc::_response(boost::asio::ip::tcp::socket & clnt, const char * rzt, size_t rzt_size)
{
    boost::system::error_code errcode;
    //int n = iegad::io::send_n(clnt, rzt, rzt_size, errcode);
    std::string sendstr(rzt, rzt_size);

    int n = clnt.send(
	boost::asio::buffer(iegad::string::en_cust(sendstr, iegad::io::MSG_KEY)), 0, 
	errcode);
    if (errcode || n != rzt_size) {
	return -1;
    }
    return n;
}
