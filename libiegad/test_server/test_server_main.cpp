#ifndef TEST_SERVER_MAIN_CPP
#define TEST_SERVER_MAIN_CPP


#include <iostream>
#include "nets/tcp_mt_svr.h"



int
main(int argc, char * argv[])
{
    iegad::nets::tcp_mt_svr host(6688);
    std::cout<<host.host_endpoint()<<std::endl;
    host.run(1);

    std::cin.get();
    std::cout<<"server done"<<std::endl;
    exit(0);
}



#endif // TEST_SERVER_MAIN_CPP

