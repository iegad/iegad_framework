#ifndef TCP_TEST_CXX
#define TCP_TEST_CXX



#include <iegad/net/tcp_server.hpp>
#include <iegad/net/tcp_client.hpp>



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



void
readHandler(iegad::net::TcpClient *clnt)
{
    char buff[1024] = {0};
    int n = clnt->readAll(buff, 1024);
    std::cout<<std::string(buff, n)<<std::endl;
}




int
main(int argc, char **)
{
    if (argc > 1) {
        iegad::net::TcpClient clnt("127.0.0.1", "6688", readHandler);

        std::cout<<clnt.getEndPoint()<<std::endl;

        for (int i = 0; i < 3; i++) {
            int n = clnt.write("Hello world", 11);
            if (n != 11) {
                std::cout<<"=== error ===\t"<<errno<<std::endl;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    else {
        EchoProtocol::TcpServer host("127.0.0.1", 6688);
        host.run();
    }

    return 0;
}



#endif // TCP_TEST_CXX
