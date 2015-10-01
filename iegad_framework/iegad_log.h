#ifndef __IEGAD_LOG__
#define __IEGAD_LOG__



// ============ 说明 ============
//
// @创建日期 : 2015-10-01
// @创建人 : iegad
//
// ============================
// @用途 : 用来记录, INFO, WARNING, ERROR, FATAL 
//		四种级别的日志;
//
// @PS : 因为打印日志会带来额外的系统开销, 由其是
//	      IO操作, 所以, 只在必要时打印日志.
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================





#ifdef WIN32	// for windows
#pragma comment(lib, "libglog.lib")


// mkdir command;
const char MKDIR[] = "mkdir LOG";

// info log path;
const char LOG_INF_FILE[] = ".\\LOG\\INFO_";
// error log path;
const char LOG_ERR_FILE[] = ".\\LOG\\ERROR_";
// warning log path;
const char LOG_WARN_FILE[] = ".\\LOG\\WARNING_";
// fatal log path;
const char LOG_FATAL_FILE[] = ".\\LOG\\FATAL_";


#else	    // for linux


// mkdir command;
const char MKDIR[] = "mkdir -p ./LOG";

// info log path;
const char LOG_INF_DIR[] = "./LOG/INFO_";
// error log path;
const char LOG_ERR_DIR[] = "./LOG/ERROR_";
// warning log path;
const char LOG_WARN_DIR[] = "./LOG/WARNING_";
// fatal log path;
const char LOG_FATAL_DIR[] = "./LOG/FATAL_";


#endif // #ifdef WIN32;

// glog standard;
#define GLOG_NO_ABBREVIATED_SEVERITIES


#include <string>
#include <glog/logging.h>
#include <io.h>


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


// INFO级 操作宏
#define iINFO		    LOG(INFO)

// WARNING级 操作宏
#define iWARN		    LOG(WARNING)

// ERROR级 操作宏
#define iERR		    LOG(ERROR)

// FATAL级 操作宏, 这里改称为 系统级错误
#define iSYSERR	    LOG(FATAL)


namespace iegad {
namespace common {


    class _LOG {
    // 日志类, 基于google glog
    public:
	// ============================
	// @用途 : 构造函数, 初始化glog的配置
	// @argv0 : main函数中的 环境变量 argv[0], 
	//		表示程序当前运行路径;
	// ============================
	explicit _LOG(char * argv0);


	// ============================
	// @用途 : 析构函数, 释放glog所使用的资源;
	// ============================
	~_LOG();

    private:
	// 禁用
	_LOG(const _LOG &);
	_LOG operator=(const _LOG &);
    }; // class _LOG;


} // namespace common;
} // namespace iegad;



#endif // __IEGAD_LOG__