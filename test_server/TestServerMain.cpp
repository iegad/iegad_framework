#include <boost/asio.hpp>
#include <iostream>
#include "iegad_framework.h"
#include "iegad_mysql.h"
#include "echo_svc.h"
#include "net/udp_pusher.h"


// =========== 用于内存泄漏检测 ============

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

// =========== 用于内存泄漏检测 ============



using namespace iegad::common;


/* ===================================
@ 说明 :

  整个程序的在vs2013 的配置下, 字符集选用 unicode编码方式所以, 
  所有的字符串都必需使用 std::wstring类型, 目的是为了正确的传输
  中文;

  在使用 protocol buffer 的传输时, 应将 std::wstring 转换成为utf8
  编码格式.

  在日志类的构造函数中, 会将当前字符集初始为 "chs(简体中文)";

 =================================== */




int 
main(int argc, char * argv[])
{
    using namespace iegad::net;
    iegad::common::_LOG log(argv[0]);

    // ======================== 服务端架构测试 ========================
    //echo_svc_ptr echo_svc_(new iegad::net::echo_svc(10));
    //tcp_mt_svr host("Eniac", "6688");
    //std::cout << host.host_endpoint() << std::endl;
    //host.regist_svc(echo_svc_);
    //host.run(4);

    //std::cout << "press <Enter> to exit..." << std::endl;
    //std::cin.get();
    //host.stop();
    // ======================== 服务端架构测试 ========================


    // ======================== MYSQL库测试 ========================
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

    // ======================== MYSQL库测试 ========================



    // ======================== UDP 测试 =========================

    //iegad::net::udp_svr host("127.0.0.1");
    //host.add_client("Eniac", iegad::net::udp_svr::rmt_addr_t("127.0.0.1", 12013));
    //char c = 'a';
    //host.send_one("Eniac", &c, 1);
    //host.send_all(&c, 1);

    // ======================== UDP 测试 =========================

    float f = 12.3456F, fr;
    std::string str = iegad::string::to_str(f, 7);

    fr = iegad::string::to_float(str);

    _CrtDumpMemoryLeaks(); // 用于windows 下, 内存泄漏检测;
    std::cin.get();
    exit(0);
}
