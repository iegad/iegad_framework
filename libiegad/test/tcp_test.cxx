#ifndef TCP_TEST_CXX
#define TCP_TEST_CXX


#include <iegad/net/tcp_server.hpp>



class MyProtocol {
public:
    MyProtocol() {}


    int
    readHandler(void *)
    {
        return 0;
    }
};


typedef iegad::net::tcp_server<MyProtocol> server_t;
typedef iegad::net::tcp_session<server_t> session_t;



int
main()
{
    server_t host("127.0.0.1", 6688);
    host.run();

    return 0;
}



#endif // TCP_TEST_CXX
