#include <boost/asio.hpp>
#include <iostream>
#include "iegad_framework.h"
#include "iegad_mysql.h"
#include "echo_svc.h"
#include "net/udp_pusher.h"


// =========== �����ڴ�й©��� ============

#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

// =========== �����ڴ�й©��� ============



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
    //tcp_mt_svr host("Eniac", "6688");
    //std::cout << host.host_endpoint() << std::endl;
    //host.regist_svc(echo_svc_);
    //host.run(4);

    //std::cout << "press <Enter> to exit..." << std::endl;
    //std::cin.get();
    //host.stop();
    // ======================== ����˼ܹ����� ========================


    // ======================== MYSQL����� ========================
    /*using namespace iegad::mysql;
    using namespace iegad::db;

    mysql_helper dbc;
    _CrtSetBreakAlloc(85);
    if (dbc.open("127.0.0.1", 3306, "iegad", "1111", "IOPC") != 0) {
	std::cout << "open failed" << std::endl;
    }



    db_tab tab;
    if (dbc.query("select * from usr_log_info_t", tab) != 0) {
	std::cout << "query failed" << std::endl;
    }

    for (int i = 0, n = tab.row_count(); i < n; i++) {
	for (int j = 0, m = tab[i].col_count(); j < m; j++) {
	    std::cout << tab[i][j] << "\t";
	}
	std::cout << std::endl;
    }
    tab.clear();*/

    // ======================== MYSQL����� ========================



    // ======================== UDP ���� =========================

    //iegad::net::udp_svr host("127.0.0.1");
    //host.add_client("Eniac", iegad::net::udp_svr::rmt_addr_t("127.0.0.1", 12013));
    //char c = 'a';
    //host.send_one("Eniac", &c, 1);
    //host.send_all(&c, 1);

    // ======================== UDP ���� =========================

    float f = 12.3456F, fr;
    std::string str = iegad::string::to_str(f, 7);

    fr = iegad::string::to_float(str);

    _CrtDumpMemoryLeaks(); // ����windows ��, �ڴ�й©���;
    std::cin.get();
    exit(0);
}
