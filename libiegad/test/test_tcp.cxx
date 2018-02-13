#ifndef TCP_TEST_CXX
#define TCP_TEST_CXX


#include <iegad/net/tcp_server.hpp>



class EchoProtocol {
public:
    DEFINE_TCP_SERVER(TcpServer, EchoProtocol, 8, 1000, iegad::net::TcpEventNull, int)



    EchoProtocol() {}


    int
    readHandler(void *arg)
    {
        TcpSessionPtr sess = TcpServer::castSession(arg);
        unsigned char buff[1200] = {0};
        int n = sess->readAll(buff, 1200);

        std::cout<<sess->getEndPoint()<<std::endl;

        if (n > 0) {
            sess->write(buff, n);
        }
        else {
            sess->close();
        }

        return n;
    }
}; // class EchoProtocol;






int
main()
{
    EchoProtocol::TcpServer host("127.0.0.1", 6688);
    host.run();
    return 0;
}



#endif // TCP_TEST_CXX
