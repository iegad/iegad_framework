#ifndef __IEGAD_DEFINE__
#define __IEGAD_DEFINE__


// ============ ˵�� ============
//
// @�������� : 2015-10-14
// @������ : iegad
//
// ============================
// @��; :  �ṩ������ܵķ��Ŷ���, ����Ҫ�����
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================


#include <string>


// float Ĭ����Ч����
#define FLOAT_DEFAULT_PRECISION	    6	
// double Ĭ����Ч����
#define DOUBLE_DEFAULT_PRECISION	    15	   


#ifdef WIN32	    //long double ��Ĭ����Ч����
#define DECIMAL_DEFAULT_PRECISION	    15
#else
#define DECIMAL_DEFAULT_PRECISION	    18
#endif // WIN32


//�����ַ���
const std::string ERR_STRING = "??????";
//UDP ����� ��ʼ�˿�
#define UDP_SVR_PORT		    5222
// һ��WHILEѭ���еĵȴ���ʶ
#define WAIT_FOR_SINGLE	    true
#define MAX_TIMEOUT_VALUE   16

// �ͻ��˽��ջ����С     
#define BUF_SIZE	1440
// ���ڼ���, ���ܵ� ��Կ
#define MSG_KEY	0x15


#ifdef WIN32	// for windows
#pragma comment(lib, "libglog.lib")


// ����LOG �ļ��� ���ز���
const char MKDIR[] = "mkdir LOG";

// info ��־����·��
const char LOG_INF_FILE[] = ".\\LOG\\INFO_";
// error ��־����·��
const char LOG_ERR_FILE[] = ".\\LOG\\ERROR_";
// warning ��־����·��
const char LOG_WARN_FILE[] = ".\\LOG\\WARNING_";
// fatal ��־����·��
const char LOG_FATAL_FILE[] = ".\\LOG\\FATAL_";


#else	    // for linux


// ����LOG �ļ��� ���ز���
const char MKDIR[] = "mkdir -p ./LOG";

// info ��־����·��
const char LOG_INF_DIR[] = "./LOG/INFO_";
// error ��־����·��
const char LOG_ERR_DIR[] = "./LOG/ERROR_";
// warning ��־����·��
const char LOG_WARN_DIR[] = "./LOG/WARNING_";
// fatal ��־����·��
const char LOG_FATAL_DIR[] = "./LOG/FATAL_";


#endif // WIN32;



#endif // __IEGAD_DEFINE__