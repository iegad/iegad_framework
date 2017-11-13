#ifndef __IEGAD_LOG__
#define __IEGAD_LOG__



// ============ 说明 ============
//
// @创建日期 : 2015-10-01
// @创建人 : iegad
//
// ============================
// @用途 : 用于本地日志记录, 分为debug, info, warnning和error, fatal
//        需要glog库的支持
//
// @PS : 因为打印日志会带来额外的系统开销, 由其是
//	      IO操作, 所以, 只在必要时打印日志.
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人               修改说明
// =======================================
// 2015-12-09           -- iegad             -- 简化使用
// 2016-04-11           -- iegad             -- 添加ERROR级操作
// 2017-11-14           -- iegad             -- 添加fatal, debug日志


// glog standard;
#define GLOG_NO_ABBREVIATED_SEVERITIES



// info && error 日志保存路径
#define     LOG_INF_FILE    "./LOG/INFO_"
#define     LOG_ERR_FILE    "./LOG/ERR_"
#define     LOG_WARN_FILE   "./LOG/WARNING_"


#ifdef WIN32
#define     MKDIR       "mkdir LOG"
#else
// 创建日志目录
#define     MKDIR       "mkdir -p ./LOG"
#endif

#include <string>
#include <glog/logging.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#include <stdlib.h>
#define _access access
#endif // WIN32
#include <assert.h>


#if (iINFO || iERR || iWARN)
#error iINFO or iERR or iWARN has defined;
#endif // iINFO 


// 操作宏
#define iDEBUG    DLOG(INFO)
#define iINFO     LOG(INFO)
#define iERR      LOG(ERROR)
#define iWARN     LOG(WARNING)
#define iFATAL    LOG(FATAL)


namespace iegad {
namespace tools {


class _LOG {
// 日志类, 基于google glog
public:
    // ============================
    // @用途 : 构造函数, 初始化glog的配置
    // @argv0 : main函数中的 环境变量 argv[0],
    //		表示程序当前运行路径;
    // ============================
    explicit _LOG(const char * argv0) {
        if (_access("LOG", 0) != 0) {
            assert(!system(MKDIR));
        }
        google::InitGoogleLogging(argv0);
        // set the file position;
        google::SetLogDestination(google::GLOG_INFO, LOG_INF_FILE);
        google::SetLogDestination(google::GLOG_ERROR, LOG_ERR_FILE);
        google::SetLogDestination(google::GLOG_WARNING, LOG_WARN_FILE);
        // set log file max size 20M;
        FLAGS_max_log_size = 20;
        FLAGS_logbufsecs = 0;
        //init charset;

#ifdef WIN32 // for win
        const char * charset = "chs";
#else // for linux
        const char * charset = "zh_CN.UTF-8";
#endif // WIN32
        std::locale::global(std::locale(charset));
    }


    // ============================
    // @用途 : 析构函数, 释放glog所使用的资源;
    // ============================
    ~_LOG() {
        google::ShutdownGoogleLogging();
    }

private:
    // 禁用
    _LOG(const _LOG &);
    _LOG & operator=(const _LOG &);
}; // class _LOG;



} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_LOG__
