#ifndef __IEGAD_LOG__
#define __IEGAD_LOG__



// ============ ˵�� ============
//
// @�������� : 2015-10-01
// @������ : iegad
//
// ============================
// @��; : ������¼, INFO
//
// @PS : ��Ϊ��ӡ��־����������ϵͳ����, ������
//	      IO����, ����, ֻ�ڱ�Ҫʱ��ӡ��־.
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================
// 2015-12-09		    -- iegad			��ʹ��
// 2016-04-11			-- iegad			���ERROR������


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


// ������
#define iINFO		    LOG(INFO)
#define iERR				LOG(ERROR)


namespace iegad {
namespace tools {


    class _LOG {
    // ��־��, ����google glog
    public:
	// ============================
	// @��; : ���캯��, ��ʼ��glog������
	// @argv0 : main�����е� �������� argv[0], 
	//		��ʾ����ǰ����·��;
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
	// @��; : ��������, �ͷ�glog��ʹ�õ���Դ;
	// ============================
	~_LOG() {
	    google::ShutdownGoogleLogging();
	}

    private:
	// ����
	_LOG(const _LOG &);
	_LOG & operator=(const _LOG &);
    }; // class _LOG;


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_LOG__
