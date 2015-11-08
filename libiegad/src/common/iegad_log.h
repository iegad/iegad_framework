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
#include "iegad_define.h"


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
namespace tools {


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
	_LOG & operator=(const _LOG &);
    }; // class _LOG;


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_LOG__
