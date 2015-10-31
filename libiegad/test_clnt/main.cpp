#include <iostream>
#include <string>
#include <ctime>
#include <thread>
#include "netc/basic_proxy.h"


class echo_proxy : public iegad::netc::basic_proxy {
public:
    explicit echo_proxy(const std::string & host, const std::string & svc)
        : iegad::netc::basic_proxy(host, svc) {}

    const std::string call(const std::string &msgstr) {
        int n;
        if(n = this->_connect(), n != 0) {
            return "";
        }
        if (this->_send_msg(10, 10, msgstr) != 0) {
            return "";
        }

        return this->_recv();
    }
};


enum {
    NTIMS = 250000
};


void test_func()
{
    echo_proxy echoPro("192.168.136.129", "6688");
    for (int i = 0; i < NTIMS; i++) {
        if (echoPro.call("Hello world") == "") {
            std::cout<<"failed"<<std::endl;
        }
        echoPro.close();
    }
}


int
main()
{
    std::clock_t bgn, end;
    std::vector<std::thread> threads;
    bgn = clock();
    for (int i = 0; i < 4; i++) {
        threads.push_back(std::thread(test_func));
    }

    for (int i = 0; i < 4; i++) {
        threads[i].join();
    }

    end = clock();
    std::cout.precision(15);
    std::cout<<"finished in : "<<(double)(end - bgn) / CLOCKS_PER_SEC <<" s"<<std::endl;
    exit(0);
}

