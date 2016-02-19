#include "gen-cpp/rdmServer_server.skeleton.h"
#include "common/iegad_log.hpp"
#include "rds_processor.h"


rdmServerHandler::rdmServerHandler()
    : rds_clients_(nullptr), max_threads_(10) 
{}


bool 
rdmServerHandler::Connect(const std::string& ipstr, const int32_t port, const std::string& usrid, const std::string& password)
{
    std::string addrStr = ipstr + ":" + std::to_string(port);
    try {
	if (rds_clients_->get_addr() != addrStr) {
	    delete rds_clients_;
	    rds_clients_ = new acl::redis_client_pool(addrStr.c_str(), max_threads_);
	}
	return rds_processor((acl::redis_client *)rds_clients_->peek()).Connect();
    }
    catch (std::exception & ex) {
	iINFO << ex.what() << std::endl;
    }
    return false;
}


bool 
rdmServerHandler::InitEvironment()
{
    try {
	acl::acl_cpp_init();
	return true;
    }
    catch (std::exception & ex) {
	iINFO << ex.what() << std::endl;
    }
    return false;
}


void 
rdmServerHandler::GetKeys(std::vector<RedisData_t> & _return, const std::string& qurey_str)
{
    if (rds_clients_ == nullptr) {
	return;
    }
    rds_processor handler((acl::redis_client *)rds_clients_->peek());
    handler.GetKeys(&_return, qurey_str);
}


void 
rdmServerHandler::GetValue(RedisData_t& _return, const std::string& _key__, const std::string& _type__)
{
    if (rds_clients_ == nullptr) {
	return;
    }
    rds_processor handler((acl::redis_client *)rds_clients_->peek());
    handler.GetValue(&_return, _key__, _type__);
}


bool 
rdmServerHandler::Select(const int16_t db_index)
{
    if (rds_clients_ == nullptr) {
	return false;
    }
    rds_processor handler((acl::redis_client *)rds_clients_->peek());
    return handler.Select(db_index);
}


rdmServerHandler::~rdmServerHandler()
{
    delete rds_clients_;
}

