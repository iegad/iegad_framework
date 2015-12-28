#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H


#define       GLOG_NO_ABBREVIATED_SEVERITIES
#define	    MKDIR                   "mkdir -p ./LOG"
#define	    LOG_INF_FILE       "./LOG/INFO_"


#include <vector>
#include <string>


const int RedisMaxConnect = 100;


void LOG_INIT(const char * argv0);


void LOG_RELEASE();


const std::string USR_PER_INFO = "UsrPersonal.";





void
InitEnvironment();


namespace tools {


const std::vector<std::string>
split_vct(const std::string &src, const std::string & chs);


const std::string
combine(const std::vector<std::string> & vStr, const std::string & chs);


const std::string
GenInvalidCode();


} // namespace tools




#endif // ENVIRONMENT_H

