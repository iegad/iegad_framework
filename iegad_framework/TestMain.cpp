#include <boost/asio.hpp>


#include <iostream>
#include "tcp_mt_server.h"
#include "job_worker.hpp"
#include "iegad_log.hpp"
#include "echo_svc.h"



using namespace iegad::tools;


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
    iegad::tools::_LOG log(argv[0]);
    std::shared_ptr<iegad::net::echo_svc> echo_svc_(new iegad::net::echo_svc(10));
    std::cout << echo_svc_->get_id();


    iegad::net::tcp_mt_svr host("127.0.0.1", 6688);
    host.regist_svc(echo_svc_);
    host.run(1);


    std::cout << "done" << std::endl;
    std::cin.get();
    exit(0);
}