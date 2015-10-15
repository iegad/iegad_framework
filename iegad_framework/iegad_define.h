#ifndef __IEGAD_DEFINE__
#define __IEGAD_DEFINE__


// ============ 说明 ============
//
// @创建日期 : 2015-10-14
// @创建人 : iegad
//
// ============================
// @用途 :  提供整个框架的符号定义, 特殊要求除外
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================


#include <string>


// float 默认有效数字
#define FLOAT_DEFAULT_PRECISION	    6	
// double 默认有效数字
#define DOUBLE_DEFAULT_PRECISION	    15	   


#ifdef WIN32	    //long double 的默认有效数字
#define DECIMAL_DEFAULT_PRECISION	    15
#else
#define DECIMAL_DEFAULT_PRECISION	    18
#endif // WIN32


//错误字符串
const std::string ERR_STRING = "??????";
//UDP 服务端 起始端口
#define UDP_SVR_PORT		    5222
// 一个WHILE循环中的等待标识
#define WAIT_FOR_SINGLE	    true
#define MAX_TIMEOUT_VALUE   16

// 客户端接收缓冲大小     
#define BUF_SIZE	1440
// 用于加密, 解密的 密钥
#define MSG_KEY	0x15


#ifdef WIN32	// for windows
#pragma comment(lib, "libglog.lib")


// 创建LOG 文件夹 命地参数
const char MKDIR[] = "mkdir LOG";

// info 日志保存路径
const char LOG_INF_FILE[] = ".\\LOG\\INFO_";
// error 日志保存路径
const char LOG_ERR_FILE[] = ".\\LOG\\ERROR_";
// warning 日志保存路径
const char LOG_WARN_FILE[] = ".\\LOG\\WARNING_";
// fatal 日志保存路径
const char LOG_FATAL_FILE[] = ".\\LOG\\FATAL_";


#else	    // for linux


// 创建LOG 文件夹 命地参数
const char MKDIR[] = "mkdir -p ./LOG";

// info 日志保存路径
const char LOG_INF_DIR[] = "./LOG/INFO_";
// error 日志保存路径
const char LOG_ERR_DIR[] = "./LOG/ERROR_";
// warning 日志保存路径
const char LOG_WARN_DIR[] = "./LOG/WARNING_";
// fatal 日志保存路径
const char LOG_FATAL_DIR[] = "./LOG/FATAL_";


#endif // WIN32;



#endif // __IEGAD_DEFINE__