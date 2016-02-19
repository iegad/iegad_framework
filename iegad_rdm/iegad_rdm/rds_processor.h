#ifndef __RDS_PROCESSOR__
#define __RDS_PROCESSOR__


#include <acl_cpp/lib_acl.hpp>
#include <inttypes.h>
#include "gen-cpp/redis_rdm_types.h"


#define NO_CONNECTING_ERR	"#No connecting error"
#define UNKOWN_REDIS_TYPE	"#Unkown redis type"


namespace rdm {


    class rds_processor {
    public:
	explicit rds_processor(acl::redis_client * client)
	    : handler_(client) {}
	
	bool Connect();
	void GetKeys(std::vector<RedisData_t> * _return, const std::string& qurey_str);
	void GetValue(RedisData_t * _return, const std::string & _key__, const std::string & _type__);
	bool Select(const int16_t db_index);

    private:
	void _getStringValue(RedisData_t * obj, const std::string & _key__);
	void _getListValue(RedisData_t * obj, const std::string & _key__);
	void _getSetValue(RedisData_t * obj, const std::string & _key__);
	void _getZSetValue(RedisData_t * obj, const std::string & _key__);
	void _getHashValue(RedisData_t * obj, const std::string & _key__);
	void _getValue(RedisData_t * obj, const std::string & _key__);

	const std::string _getType(const std::string & _key__);

	rds_processor(const rds_processor &);
	rds_processor & operator=(const rds_processor &);

	acl::redis_client * handler_;
    }; // class rds_processor;


} // namespace rdm;


#endif // __RDS_PROCESSOR__