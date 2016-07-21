#ifndef __IEGAD_COMMON_IN__
#define __IEGAD_COMMON_IN__



// float 精度
#define     FLOAT_DEFAULT_PRECISION         6
// double 精度
#define     DOUBLE_DEFAULT_PRECISION     15
// 错误字符串
#define     ERR_STRING      "??????"


// ===========
//	for windows
// ===========
#ifdef WIN32 
// long double 精度
#define     DECIMAL_DEFAULT_PRECISION       15
// 系统字符集
#define	    CHARSET_DEFAULT     "GBK"
// 创建日志目录
#define	    MKDIR       "mkdir LOG"


// ===========
//	for linux
// ===========
#else 
// long double 精度
#define	    DECIMAL_DEFAULT_PRECISION       18
// 系统字符集
#define	    CHARSET_DEFAULT     "zh_CN.UTF-8"
// 创建日志目录
#define	    MKDIR       "mkdir -p ./LOG"
// 使 windows 和 linux 下统一调用名称
#define	    _snprintf(buff, buffsize, fmt, ...)	    { snprintf((buff), (buffsize), (fmt), ##__VA_ARGS__); }
#endif // WIN32


// info && error 日志保存路径
#define	    LOG_INF_FILE      "./LOG/INFO_"
#define		LOG_ERR_FILE      "./LOG/ERR_"
#define     LOG_WARN_FILE     "./LOG/WARNING_"


#endif // __IEGAD_COMMON_IN__
