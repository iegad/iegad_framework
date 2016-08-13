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
    std::unique_lock<std::mutex> lk(mtx_);
    static int i = 1;
    std::cout<<i++<<std::endl;
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
<<<<<<< HEAD
   return RUN_ALL_TESTS();
=======

    if (argc > 1) {
        // client
        std::cout<<"client\n";
        std::vector<std::thread> ts;
        for (int i = 0, n = std::stoi(argv[1]); i < n; i++) {
            ts.push_back(std::thread(std::bind(client, std::stoi(argv[2]))));
        }

        for (int i = 0, n = ts.size(); i < n; i++) {
            if (ts[i].joinable()) {
                ts[i].join();
            }
        }
    }
    else {
        // server
        iegad::net::tcp_server::que_t que;
        iegad::tools::worker_t<iegad::net::msg_t::ptr_t> wkr(que, checkMsg);
        wkr.run(1);
        iegad::net::tcp_server::ptr_t host = iegad::net::tcp_server::Create(6688, que, 8);
        host->run();
    }
    std::cout<<"done\n";
    exit(0);
    //return RUN_ALL_TESTS();
>>>>>>> 8656b058c030cc3a9ebf2355859034ce2095e9e2
}

