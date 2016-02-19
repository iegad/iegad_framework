#include "rds_processor.h"
#include "common/iegad_log.hpp"


bool 
rdm::rds_processor::Connect()
{
    try {
	bool res = false;
	acl::redis_connection cmd(handler_);
	if (!(res = cmd.ping())) {
	    iINFO << "Connect Redis : " << cmd.get_client_addr() << "failed" << std::endl;
	}
	return res;
    }
    catch (std::exception & ex) {
	iINFO << ex.what() << std::endl;
    }
    return false;
}


void 
rdm::rds_processor::GetKeys(std::vector<RedisData_t> * _return, const std::string& qurey_str)
{
    try {
	acl::redis_key cmd(handler_);
	std::vector<acl::string> resVct;
	if (cmd.keys_pattern(qurey_str.c_str(), &resVct) < 0) {
	    iINFO << cmd.result_error() << std::endl;
	    return;
	}
	for (auto itor = resVct.begin(); itor != resVct.end(); itor++) {
	    RedisData_t temp;
	    temp.__set_key(itor->c_str());
	    temp.__set_type(this->_getType(temp.key));
	    _return->push_back(temp);
	}
    }
    catch (std::exception & ex) {
	iINFO << ex.what() << std::endl;
	_return->clear();
    }
}


void 
rdm::rds_processor::GetValue(RedisData_t * _return, const std::string & _key__, const std::string & _type__)
{
    try {
	_return->__set_type(_type__);
	if (_type__ == "string") {	    
	    this->_getStringValue(_return, _key__);
	}
	else if (_type__ == "list") {
	    this->_getListValue(_return, _key__);
	}
	else if (_type__ == "set") {
	    this->_getSetValue(_return, _key__);
	}
	else if (_type__ == "zset") {
	    this->_getZSetValue(_return, _key__);
	}
	else if (_type__ == "hash") {
	    this->_getHashValue(_return, _key__);
	}
	else {
	    this->_getValue(_return, _key__);
	}
    }
    catch (std::exception & ex) {
	iINFO << ex.what() << std::endl;
    }
}


bool 
rdm::rds_processor::Select(const int16_t db_index)
{
    try {
	bool res = false;
	acl::redis_connection cmd(handler_);
	if ((res = cmd.select(db_index)) == false) {
	    iINFO << cmd.result_error() << std::endl;
	}
	return res;
    }
    catch (std::exception & ex) {
	iINFO << ex.what() << std::endl;
    }
    return false;
}


void 
rdm::rds_processor::_getStringValue(RedisData_t * obj, const std::string & _key__)
{
    acl::redis_string cmd(handler_);
    acl::string res;
    if (!cmd.get(_key__.c_str(), res)) {
	iINFO << cmd.result_error() << std::endl;
    }
    obj->__set_key(_key__);
    obj->__set_str_value(res.c_str());
}


void 
rdm::rds_processor::_getListValue(RedisData_t * obj, const std::string & _key__)
{
    acl::redis_list cmd(handler_);
    std::vector<acl::string> resVct;
    std::vector<std::string> listValue;
    if (!cmd.lrange(_key__.c_str(), 0, -1, &resVct)) {
	iINFO << cmd.result_error() << std::endl;
    }

    for (auto itor = resVct.begin(); itor != resVct.end(); itor++) {
	listValue.push_back(itor->c_str());
    }
    obj->__set_lzs_value(listValue);
}


void 
rdm::rds_processor::_getSetValue(RedisData_t * obj, const std::string & _key__)
{
    acl::redis_set cmd(handler_);
    std::vector<acl::string> resVct;
    std::vector<std::string> setValue;
    if (cmd.smembers(_key__.c_str(), &resVct) < 0) {
	iINFO << cmd.result_error() << std::endl;
    }
    for (auto itor = resVct.begin(); itor != resVct.end(); itor++) {
	setValue.push_back(itor->c_str());
    }
    obj->__set_lzs_value(setValue);
}


void 
rdm::rds_processor::_getZSetValue(RedisData_t * obj, const std::string & _key__)
{
    acl::redis_zset cmd(handler_);
    std::vector<acl::string> resVct;
    std::vector<std::string> zsetValue;
    if (cmd.zrange(_key__.c_str(), 0, -1, &resVct) < 0) {
	iINFO << cmd.result_error() << std::endl;
    }
    for (auto itor = resVct.begin(); itor != resVct.end(); itor++) {
	zsetValue.push_back(itor->c_str());
    }
    obj->__set_lzs_value(zsetValue);
}


void 
rdm::rds_processor::_getHashValue(RedisData_t * obj, const std::string & _key__)
{
    acl::redis_hash cmd(handler_);
    std::map<acl::string, acl::string> mapValue;
    std::map<std::string, std::string> hashValue;
    if (!cmd.hgetall(_key__.c_str(), mapValue)) {
	iINFO << cmd.result_error() << std::endl;
    }
    for (auto itor = mapValue.begin(); itor != mapValue.end(); itor++) {
	hashValue.insert(std::make_pair(itor->first.c_str(), itor->second.c_str()));
    }
    obj->__set_hash_value(hashValue);
}


const std::string 
rdm::rds_processor::_getType(const std::string & _key__)
{
    std::string res;
    acl::redis_key cmd(handler_);
    switch (cmd.type(_key__.c_str())) {
    case acl::REDIS_KEY_STRING:
	res = "string";
	break;
    case acl::REDIS_KEY_LIST:
	res = "list";
	break;
    case acl::REDIS_KEY_SET:
	res = "set";
	break;
    case acl::REDIS_KEY_ZSET:
	res = "zset";
	break;
    case acl::REDIS_KEY_HASH:
	res = "hash";
	break;
    default:
	res = UNKOWN_REDIS_TYPE;
	break;
    }
    return res;
}


void 
rdm::rds_processor::_getValue(RedisData_t * obj, const std::string & _key__)
{
    obj->__set_type(this->_getType(_key__));

    if (obj->type == "string") {
	this->_getStringValue(obj, _key__);
    }
    else if (obj->type == "list") {
	this->_getListValue(obj, _key__);
    }
    else if (obj->type == "set") {
	this->_getSetValue(obj, _key__);
    }
    else if (obj->type == "zset") {
	this->_getZSetValue(obj, _key__);
    }
    else if (obj->type == "hash") {
	this->_getHashValue(obj, _key__);
    }
}
