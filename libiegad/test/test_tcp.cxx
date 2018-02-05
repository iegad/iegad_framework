#ifndef TCP_TEST_CXX
#define TCP_TEST_CXX


#include <iegad/net/tcp_server.hpp>



class MyProtocol {
public:
    typedef iegad::net::tcp_server<MyProtocol> server_t;
    typedef iegad::net::tcp_session<server_t> session_t;
    typedef session_t* session_ptr;

    MyProtocol() {}


    int
    readHandler(void *arg)
    {
        session_ptr sess = (session_ptr)arg;
        char buff[1200] = {0};
        int n = read(sess->sockfd(), buff, 1200);
        if (n > 0) {
            std::cout<<std::string(buff, n)<<std::endl;
            n = write(sess->sockfd(), buff, n);
            sess->close();
        }

        return n;
    }
}; // class MyProtocol;






int
main()
{
    MyProtocol::server_t host("127.0.0.1", 6688);
    host.run();

    return 0;
}



#endif // TCP_TEST_CXX
