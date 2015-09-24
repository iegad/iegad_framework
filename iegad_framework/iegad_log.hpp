#ifndef __IEGAD_LOG__
#define __IEGAD_LOG__



#ifdef WIN32
#pragma comment(lib, "libglog.lib")


const char MKDIR[] = "mkdir LOG";

const char LOG_INF_FILE[] = ".\\LOG\\INFO_";
const char LOG_ERR_FILE[] = ".\\LOG\\ERROR_";
const char LOG_WARN_FILE[] = ".\\LOG\\WARNING_";
const char LOG_FATAL_FILE[] = ".\\LOG\\FATAL_";

#else


const char MKDIR[] = "mkdir -p ./LOG";

const char LOG_INF_DIR[] = "./INFO_";
const char LOG_ERR_DIR[] = "./ERROR_";
const char LOG_WARN_DIR[] = "./WARNING_";
const char LOG_FATAL_DIR[] = "./FATAL_";


#endif

#define GLOG_NO_ABBREVIATED_SEVERITIES

#include <string>
#include <glog/logging.h>
#include <io.h>



namespace iegad {
namespace tools {


    class _LOG {
    public:
	_LOG(char * argv0) {
	    if (access("LOG", 0) != 0) {
		system(MKDIR);
	    }
	    google::InitGoogleLogging(argv0);
	    google::SetLogDestination(google::GLOG_INFO, LOG_INF_FILE);
	    google::SetLogDestination(google::GLOG_WARNING, LOG_WARN_FILE);
	    google::SetLogDestination(google::GLOG_ERROR, LOG_ERR_FILE);
	    google::SetLogDestination(google::GLOG_FATAL, LOG_FATAL_FILE);
	    FLAGS_max_log_size = 100;
	}

	~_LOG() {
	    google::ShutdownGoogleLogging();
	}
    };


#ifdef iINFO 
#error iINFO has defined;
#endif // iINFO 

#ifdef iWARN 
#error iWARN has defined;
#endif // iWARN 

#ifdef iERR 
#error iERR has defined;
#endif // iERR 

#ifdef iSYSERR 
#error iSYSERR has defined;
#endif // iSYSERR 


#define iINFO		    LOG(INFO)
#define iWARN		    LOG(WARNING)
#define iERR		    LOG(ERROR)
#define iSYSERR	    LOG(FATAL)


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_LOG__