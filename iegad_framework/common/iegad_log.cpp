#include "iegad_log.h"



iegad::common::_LOG::_LOG(char * argv0)
{
    if (access("LOG", 0) != 0) {
	system(MKDIR);
    }
    google::InitGoogleLogging(argv0);
    // set the file position;
    google::SetLogDestination(google::GLOG_INFO, LOG_INF_FILE);
    google::SetLogDestination(google::GLOG_WARNING, LOG_WARN_FILE);
    google::SetLogDestination(google::GLOG_ERROR, LOG_ERR_FILE);
    google::SetLogDestination(google::GLOG_FATAL, LOG_FATAL_FILE);
    // set log file max size 100M;
    FLAGS_max_log_size = 100;
    //init charset;
    std::locale::global(std::locale("chs"));
}


iegad::common::_LOG::~_LOG()
{
    google::ShutdownGoogleLogging();
}
