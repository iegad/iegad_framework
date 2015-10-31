#ifndef TEST_SERVER_MAIN_CPP
#define TEST_SERVER_MAIN_CPP


#include <iostream>
#include "nets/tcp_mt_svr.h"
#include "echo_svc.hpp"




int
main(int argc, char * argv[])
{
    std::shared_ptr<echo_svc> echoSvc(new echo_svc);

    iegad::nets::tcp_mt_svr host("192.168.136.129", "6688");
    std::cout<<host.host_endpoint()<<std::endl;
    host.regist_svc(echoSvc);
    host.run(4);

    std::cin.get();
    host.stop();
    std::cout<<"server done"<<std::endl;
    exit(0);
}



#endif // TEST_SERVER_MAIN_CPP

