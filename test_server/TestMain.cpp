#include <boost/asio.hpp>

// =========== 用于内存泄漏检测 ============

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// =========== 用于内存泄漏检测 ============


#pragma comment(lib, "iegad_framework.lib")


#include <iostream>
#include "net/tcp_mt_server.h"
#include "common/iegad_log.h"
#include "echo_svc.h"
#include "mysql_helper.h"


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
    //tcp_mt_svr host("127.0.0.1", 6688);
    //host.regist_svc(echo_svc_);
    //host.run(1);

    //std::cout << "press <Enter> to exit..." << std::endl;
    //std::cin.get();
    //host.stop();
    // ======================== 服务端架构测试 ========================


    // ======================== MYSQL库测试 ========================
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
    // ======================== MYSQL库测试 ========================

    _CrtDumpMemoryLeaks(); // 用于windows 下, 内存泄漏检测;
    std::cin.get();
    exit(0);
}
