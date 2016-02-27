#include "thrift_IElefunImp.h"
#include "common/iegad_string.hpp"


void 
iegad::thrift_ex::IElefunImp::Action(res_t * _return, const req_t & obj)
{
    if (obj.__isset.r_md5) {
	if (!_md5_check(obj.r_str, obj.r_size, obj.r_md5)) {
	    _return->r_errstr = "MD5 INVALIED";
	    _return->r_type = -1;
	    return;
	}
    } // if (obj.__isset.r_md5);

    if (obj.__isset.r_sha1) {
	if (!obj.__isset.r_md5 || !_sha1_check(obj.r_md5, obj.r_sha1)) {
	    _return->r_errstr = "SHA1 INVALIED";
	    _return->r_type = -1;
	    return;
	}
    } // if (obj.__isset.r_sha1);

    _return->__set_r_type(_action(obj.r_str, obj.r_size,
	&_return->r_str, &_return->r_size,
	&_return->r_errcode, &_return->r_errstr));

    if (!_return->r_str.empty()) {
	_return->__isset.r_str = true;
    }

    if (!_return->r_errstr.empty()) {
	_return->__isset.r_errstr = true;
    }

    if (_return->r_errcode != 0) {
	_return->__isset.r_errcode = true;
    }

    if (_return->r_size != 0) {
	_return->__isset.r_size = true;
    }
}


bool 
iegad::thrift_ex::IElefunImp::_md5_check(const std::string & msgstr, int32_t msgsize, const std::string & md5str)
{
    std::string checkstr = msgsize > 0 ? std::string(msgstr.c_str(), msgsize) : msgstr;
    checkstr = iegad::string::md5(checkstr);
    return checkstr.size() == md5str.size() && checkstr == md5str;
}


bool 
iegad::thrift_ex::IElefunImp::_sha1_check(const std::string & md5str, const std::vector<int32_t> & sha1vct)
{
    do {
	std::vector<uint32_t> checkVct;
	if (sha1vct.size() != 5) {
	    break;
	}

	iegad::string::sha1(md5str, checkVct);

	for (int i = 0; i < 5; i++) {
	    if (sha1vct[i] != (int32_t)checkVct[i]) {
		return false;
	    }
	}

	return true;
    } while (false);
    return false;
}
