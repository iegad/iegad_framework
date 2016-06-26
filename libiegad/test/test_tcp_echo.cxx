#include <iostream>
#include "net/iegad_tcp_server.hpp"
#include "container/iegad_safe_map.hpp"
#include <arpa/inet.h>
#include <thread>



class EchoServer {
public:
    static void OnConnect(std::shared_ptr<iegad::net::tcp_session> tcpSession) {
        std::cout << tcpSession->sockfd() << " : connected\n";
        m_.insert(std::make_pair(tcpSession->sockfd(), tcpSession));
    }

    static void OnReadErr(int sockfd) {
        std::cout << sockfd << " : readerr\n";
        auto itr = m_.find(sockfd);
        if (itr == m_.end()) {
            return;
        }
        m_.erase(itr);
    }

    static void OnReadEof(int sockfd) {
        std::cout << sockfd << " : readeof\n";
        iegad::map<int, std::shared_ptr<iegad::net::tcp_session>>::iterator itr = m_.find(sockfd);
        if (itr == m_.end()) {
            return;
        }
        m_.erase(itr);
        std::cout << m_.size() << std::endl;
    }


    static int OnRead(int sockfd, const char * buff, size_t n) {
        auto itr = m_.find(sockfd);
        std::shared_ptr<iegad::net::tcp_session> s = itr->second;
        s->setMsgbuff(std::string(buff, n));
        std::cout<<s->msgbuff()<<std::endl;
        s->msgbuff().clear();
        return 0;
    }


    EchoServer(uint16_t port)
        :
          host_(port,
                &EchoServer::OnConnect,
                &EchoServer::OnRead,
                &EchoServer::OnReadEof,
                &EchoServer::OnReadErr)
    {}


    void run() {
        host_.run();
    }



private:
    iegad::net::tcp_server host_;
    static std::map<int, std::shared_ptr<iegad::net::tcp_session>> m_;
}; // class EchoServer;


std::map<int, std::shared_ptr<iegad::net::tcp_session>> EchoServer::m_;


int
main(int argc, char * argv[])
{
    if (argc > 2) {
        for (;;) {
            std::cout<< "Client" << std::endl;
            int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            assert(sockfd > 0);
            sockaddr_in addr;
            memset(&addr, 0, sizeof(addr));
            addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            addr.sin_family = AF_INET;
            addr.sin_port = htons(6688);
            assert(!connect(sockfd, (sockaddr *)&addr, sizeof(addr)));
            for (int i = 0, n = std::stoi(argv[1]); i < n; i++)
                write(sockfd, "hello world\n", 12);
            close(sockfd);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    else {
        std::cout<< "Server" << std::endl;
        EchoServer host(6688);
        host.run();
    }
    exit(0);
}
