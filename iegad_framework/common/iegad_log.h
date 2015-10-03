#ifndef __IEGAD_LOG__
#define __IEGAD_LOG__



// ============ ˵�� ============
//
// @�������� : 2015-10-01
// @������ : iegad
//
// ============================
// @��; : ������¼, INFO, WARNING, ERROR, FATAL 
//		���ּ������־;
//
// @PS : ��Ϊ��ӡ��־����������ϵͳ����, ������
//	      IO����, ����, ֻ�ڱ�Ҫʱ��ӡ��־.
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
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


// INFO�� ������
#define iINFO		    LOG(INFO)

// WARNING�� ������
#define iWARN		    LOG(WARNING)

// ERROR�� ������
#define iERR		    LOG(ERROR)

// FATAL�� ������, ����ĳ�Ϊ ϵͳ������
#define iSYSERR	    LOG(FATAL)


namespace iegad {
namespace common {


    class _LOG {
    // ��־��, ����google glog
    public:
	// ============================
	// @��; : ���캯��, ��ʼ��glog������
	// @argv0 : main�����е� �������� argv[0], 
	//		��ʾ����ǰ����·��;
	// ============================
	explicit _LOG(char * argv0);


	// ============================
	// @��; : ��������, �ͷ�glog��ʹ�õ���Դ;
	// ============================
	~_LOG();

    private:
	// ����
	_LOG(const _LOG &);
	_LOG operator=(const _LOG &);
    }; // class _LOG;


} // namespace common;
} // namespace iegad;



#endif // __IEGAD_LOG__