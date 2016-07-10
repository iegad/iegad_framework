#include <iostream>
#include "net/iegad_tcp_server.hpp"
#include "container/iegad_safe_map.hpp"
#include <arpa/inet.h>
#include <thread>



class EchoServer : public iegad::net::tcp_event {
public:
    int onReadBuff(int sockfd, const char *buff, size_t n) {
        auto itr = sm_.find(sockfd);
        std::shared_ptr<iegad::net::tcp_session> s = itr->second;
        s->setMsgbuff(std::string(buff, n));
        std::cout<<s->msgbuff()<<std::endl;
        s->msgbuff().clear();
        return 0;
    }


    EchoServer(uint16_t port)
        :
          host_(port, this)
    {}


    void run() {
        host_.run();
    }



private:
    iegad::net::tcp_server host_;
}; // class EchoServer;



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
