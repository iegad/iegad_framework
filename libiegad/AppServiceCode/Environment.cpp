


#include "Environment.h"
#include <unistd.h>
#include <stdlib.h>
#include <glog/logging.h>
#include <ctime>
#include <acl_cpp/lib_acl.hpp>


void
LOG_INIT(const char * argv0)
{
    if (access("LOG", 0) != 0) {
        system(MKDIR);
    }
    google::InitGoogleLogging(argv0);
    // set the file position;
    google::SetLogDestination(google::GLOG_INFO, LOG_INF_FILE);
    // set log file max size 100M;
    FLAGS_max_log_size = 100;
    //init charset;
    const char * charset = "zh_CN.UTF-8";
    std::locale::global(std::locale(charset));
}


void
LOG_RELEASE()
{
    google::ShutdownGoogleLogging();
}


const std::vector<std::string>
tools::split_vct(const std::string &src, const std::string &chs)
{
    int pos = 0, len = chs.length(), n = 0, index = 0;
    std::vector<std::string> res;
    while (true) {
        index = src.find(chs, pos);
        if (index == -1) {
            res.push_back(std::string(src, pos, src.length() - pos));
            break;
        }
        n = index - n;
        res.push_back(std::string(src, pos, n));
        pos = index + len;
        n = pos;
    }
    return res;
}


const std::string
tools::combine(const std::vector<std::string> &vStr, const std::string &chs)
{
    std::string res;
    for (int i = 0, n = vStr.size() - 1; i < n; i++) {
        res += vStr[i] + chs;
    }
    return res;
}


const std::string
tools::GenInvalidCode()
{
    time_t timeNow = std::time(nullptr);
    std::string res = std::to_string(timeNow + std::rand() + 123456);
    return res.substr(0, 6);
}


void
InitEnvironment()
{
    acl::acl_cpp_init();
}
