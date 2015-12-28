#ifndef __IEGAD_COMMON_IN__
#define __IEGAD_COMMON_IN__


// float ����
#define	    FLOAT_DEFAULT_PRECISION	    6
// double ����
#define	    DOUBLE_DEFAULT_PRECISION	    15
// �����ַ���
#define	    ERR_STRING					    "??????"


// ===========
//	for windows
// ===========
#ifdef WIN32 
// long double ����
#define	    DECIMAL_DEFAULT_PRECISION	    15
// ϵͳGBK �ַ���
#define	    CHARSET_DEFAULT			    "GBK"


// ������־Ŀ¼
#define	    MKDIR			    "mkdir LOG"
// info ��־����·��
#define	    LOG_INF_FILE	    ".\\LOG\\INFO_"
// error ��־����·��
#define	    LOG_ERR_FILE	    ".\\LOG\\ERROR_"
// warning ��־����·��
#define	    LOG_WARN_FILE	    ".\\LOG\\WARNING_"
// fatal ��־����·��
#define	    LOG_FATAL_FILE	    ".\\LOG\\FATAL_"


// ===========
//	    for linux
// ===========
#else 
// long double ����
#define	    DECIMAL_DEFAULT_PRECISION	    18
// ϵͳGBK �ַ���
#define	    CHARSET_DEFAULT			    "zh_CN.GBK"

// ������־Ŀ¼
#define	    MKDIR			    "mkdir -p ./LOG";
// info ��־����·��
#define	    LOG_INF_FILE	    "./LOG/INFO_"
// error ��־����·��
#define	    LOG_ERR_FILE	    "./LOG/ERROR_"
// warning ��־����·��
#define	    LOG_WARN_FILE	    "./LOG/WARNING_"
// fatal ��־����·��
#define	    LOG_FATAL_FILE	    "./LOG/FATAL_"

// ʹ windows �� linux ��ͳһ��������
#define	    _snprintf(buff, buffsize, fmt, ...)	    { snprintf((buff), (buffsize), (fmt), ##__VA_ARGS__); }
#endif // WIN32



#endif // __IEGAD_COMMON_IN__