#include <boost/asio.hpp>

// =========== �����ڴ�й©��� ============

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// =========== �����ڴ�й©��� ============


#pragma comment(lib, "iegad_framework.lib")


#include <iostream>
#include "net/tcp_mt_server.h"
#include "common/iegad_log.h"
#include "echo_svc.h"
#include "mysql_helper.h"


using namespace iegad::common;


/* ===================================
@ ˵�� :

  �����������vs2013 ��������, �ַ���ѡ�� unicode���뷽ʽ����, 
  ���е��ַ���������ʹ�� std::wstring����, Ŀ����Ϊ����ȷ�Ĵ���
  ����;

  ��ʹ�� protocol buffer �Ĵ���ʱ, Ӧ�� std::wstring ת����Ϊutf8
  �����ʽ.

  ����־��Ĺ��캯����, �Ὣ��ǰ�ַ�����ʼΪ "chs(��������)";

 =================================== */


int 
main(int argc, char * argv[])
{
    using namespace iegad::net;
    iegad::common::_LOG log(argv[0]);

    // ======================== ����˼ܹ����� ========================
    //echo_svc_ptr echo_svc_(new iegad::net::echo_svc(10));
    //tcp_mt_svr host("127.0.0.1", 6688);
    //host.regist_svc(echo_svc_);
    //host.run(1);

    //std::cout << "press <Enter> to exit..." << std::endl;
    //std::cin.get();
    //host.stop();
    // ======================== ����˼ܹ����� ========================


    // ======================== MYSQL����� ========================
    using namespace iegad::mysql;
    using namespace iegad::db;

    mysql_helper dbc;
    if (dbc.open("Eniac", 3306, "iegad", "1111", "iegad_api_db") != 0) {
	std::cout << "open failed" << std::endl;
    }

    dbtab_ptr tab(new db_tab);
    if (dbc.query("select * from employee_t", tab) != 0) {
	std::cout << "query failed" << std::endl;
    }

    for (int i = 0, n = tab->row_count(); i < n; i++) {
	for (int j = 0, m = (*tab)[i]->col_count(); j < m; j++) {
	    std::string val = (*(*tab)[i])[j];
	    std::cout << val << "\t";
	}
	std::cout << std::endl;
    }
    // ======================== MYSQL����� ========================

    _CrtDumpMemoryLeaks(); // ����windows ��, �ڴ�й©���;
    std::cin.get();
    exit(0);
}
