#ifndef __IEGAD_LOG__
#define __IEGAD_LOG__



// ============ 说明 ============
//
// @创建日期 : 2015-10-01
// @创建人 : iegad
//
// ============================
// @用途 : 用来记录, INFO
//
// @PS : 因为打印日志会带来额外的系统开销, 由其是
//	      IO操作, 所以, 只在必要时打印日志.
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================
// 2015-12-09		    -- iegad			简化使用
// 2016-04-11			-- iegad			添加ERROR级操作


// glog standard;
#define GLOG_NO_ABBREVIATED_SEVERITIES


#include <string>
#include <glog/logging.h>
#ifdef WIN32
#include <io.h>
#else
#include <unistd.h>
#define _access access
#endif // WIN32
#include "iegad_common.in.h"


#ifdef iINFO || iERR
#error iINFO or iERR has defined;
#endif // iINFO 


// 操作宏
#define iINFO		    LOG(INFO)
#define iERR				LOG(ERROR)


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
	explicit _LOG(char * argv0) {
	    if (_access("LOG", 0) != 0) {
		system(MKDIR);
	    }
	    google::InitGoogleLogging(argv0);
	    // set the file position;
	    google::SetLogDestination(google::GLOG_INFO, LOG_INF_FILE);
		google::SetLogDestination(google::GLOG_ERROR, LOG_ERR_FILE);
	    // set log file max size 100M;
	    FLAGS_max_log_size = 100;
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
