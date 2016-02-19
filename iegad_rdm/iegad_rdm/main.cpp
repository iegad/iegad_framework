#define XXX_Handler		rdmServerHandler
#define XXX_If				rdmServerIf
#define XXX_IfFactory		rdmServerIfFactory
#define XXX_ProcessorFactory	rdmServerProcessorFactory


#include "gen-cpp/rdmServer_server.skeleton.h"
#include "common/thrift_ex.hpp"
#include "common/iegad_log.hpp"
#include <pugixml/pugixml.hpp>


#include "acl_cpp/redis/redis_client.hpp"
#include "rds_processor.h"
#include <iostream>


int 
main(int argc, char * argv[])
{
    iegad::tools::_LOG logObj(argv[0]);

    do {
	if (!ServerHost::InitEvironment()) {
	    break;
	}
	acl::redis_client clnt("192.168.1.210:6379");
	rds_processor proc(&clnt);
	proc.Connect();
	std::vector<RedisData_t> dataVct;
	proc.GetKeys(&dataVct, "*");
	for (auto itor = dataVct.begin(); itor != dataVct.end(); itor++) {
	    std::cout << itor->key << '\t' << itor->type << std::endl;
	}
	std::cin.get();
	exit(0);
    } while (false);
    exit(1);
}
