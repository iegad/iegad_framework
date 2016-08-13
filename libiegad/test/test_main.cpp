#include <iostream>
#include <clocale>
#include <locale>
#include <gtest/gtest.h>
#include "net/iegad_tcp_server.hpp"
#include "tools/job_worker.hpp"
#include <atomic>


std::mutex mtx_;


int
checkMsg(iegad::net::msg_t::ptr_t msg) {
    std::cout<<msg->m_data<<std::endl;
    return 0;
}


void client(int t) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::string msgstr = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(6688);
    addr.sin_family = AF_INET;
    if (connect(sockfd, (sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cout<<"connected "<<errno<<std::endl;
        return;
    }
    iegad::net::msg_t msg;
    msg.m_head.msg_size = msgstr.size();
    msg.m_data = msgstr;
    int n;
    for (int i = 0; i < t; i++) {
        n = write(sockfd, &msg.m_head, sizeof(msg.m_head));
        if (n < 0) {
            std::cout<<"head : "<<errno<<std::endl;
            break;
        }
        n = write(sockfd, msg.m_data.c_str(), msg.m_data.size());
        if (n < 0) {
            std::cout<<"data : "<<errno<<std::endl;
            break;
        }
    }
    close(sockfd);
}


int
main(int argc, char * argv[])
{
    std::setlocale(LC_ALL, "zh_CN.UTF-8");
    testing::InitGoogleTest(&argc, argv);
    if (argc > 1) {
        std::cout<<"client\n";
        client(10);
    }
    else {
        std::cout<<"server\n";
        iegad::tools::job_que_t<iegad::net::msg_t::ptr_t> que;
        iegad::tools::worker_t<iegad::net::msg_t::ptr_t> wkr(que, checkMsg);
        wkr.run(1);
        iegad::net::tcp_server::ptr_t host = iegad::net::tcp_server::Create(6688, que);
        host->run();
    }
    return 0;//RUN_ALL_TESTS();
}

