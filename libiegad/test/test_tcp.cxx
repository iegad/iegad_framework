#ifndef TCP_TEST_CXX
#define TCP_TEST_CXX



#include <iegad/net/tcp_server.hpp>




int
main(int argc, char **)
{
    iegad::net::TcpServer svr;
    svr.run();
    exit(0);
}



#endif // TCP_TEST_CXX
