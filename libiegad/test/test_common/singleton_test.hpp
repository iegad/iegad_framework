#ifndef __SINGLETON_TEST__
#define __SINGLETON_TEST__


#include <thread>
#include <vector>
#include <iostream>
#include "common/iegad_singleton.hpp"


class hello {
public:
    void fun() {
	std::cout << "hello world" << std::endl;
    }
};


void
singleton_test() {
    std::vector<std::thread> thrdVct;
    hello * ph = iegad::tools::singleton<hello>::instance();
    for (int i = 0; i < 100; i++) {
	thrdVct.push_back(std::thread(std::bind(&hello::fun, ph)));
    }

    for (int i = 0; i < 100; i++) {
	thrdVct[i].join();
    }
    std::cout << "[ Singleton test DONE ]" << std::endl;
}



#endif // __SINGLETON_TEST__