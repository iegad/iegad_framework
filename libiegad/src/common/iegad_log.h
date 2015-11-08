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


// INFO�� ������
#define iINFO		    LOG(INFO)

// WARNING�� ������
#define iWARN		    LOG(WARNING)

// ERROR�� ������
#define iERR		    LOG(ERROR)

// FATAL�� ������, ����ĳ�Ϊ ϵͳ������
#define iSYSERR	    LOG(FATAL)


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
	explicit _LOG(char * argv0);


	// ============================
	// @��; : ��������, �ͷ�glog��ʹ�õ���Դ;
	// ============================
	~_LOG();

    private:
	// ����
	_LOG(const _LOG &);
	_LOG & operator=(const _LOG &);
    }; // class _LOG;


} // namespace tools;
} // namespace iegad;



#endif // __IEGAD_LOG__
