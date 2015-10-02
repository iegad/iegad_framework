#include <boost/asio.hpp>


#include <iostream>
#include "tcp_mt_server.h"
#include "job_worker.hpp"
#include "iegad_log.h"
#include "echo_svc.h"
#include "mysql_conn.h"
#include "iegad_string.h"



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
    iegad::common::_LOG log(argv[0]);
    //std::shared_ptr<iegad::net::echo_svc> echo_svc_(new iegad::net::echo_svc(10));

    //iegad::net::tcp_mt_svr host("127.0.0.1", 6688);
    //host.regist_svc(echo_svc_);
    //host.run(1);

    //std::cout << "press <Enter> to exit..." << std::endl;
    //std::cin.get();
    //host.stop();

    using namespace iegad::mysql;

    mysql_conn conn("Eniac", 3306, "iegad", "1111", "iegad_api_db");
    if (conn.failed()) {
	std::cout << "failed" << std::endl;
    }

    int n = conn.exec("insert into employee_t (E_NICK, E_NAME) values ('Xiaoqi', 'iegad')");
    if (n != 1) {
	std::cout << conn.err_msg() << std::endl;
    }

    std::queue<std::string> sqlque;
    for (int i = 0; i < 10; i++) {
	std::string str = "insert into employee_t (E_NICK, E_NAME) values ('" + iegad::string::to_str(i) + "', '" + iegad::string::to_str(i) + "')";
	sqlque.push(str);
    }

    n = conn.exec_trans(sqlque);

    mysql_rzt rzt(conn, conn.query("select * from employee_t"));
    if (conn.failed()) {
	std::cout << conn.err_code() << std::endl;
    }

    MYSQL_ROW row;
    while ((row = rzt.read_row())) {
	std::cout << row[0] << "\t\t" << row[1] << std::endl;
    }
    if (conn.failed()) {
	std::cout << conn.err_code() << std::endl;
    }


    std::cin.get();
    exit(0);
}
