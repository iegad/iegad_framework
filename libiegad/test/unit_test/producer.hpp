#ifndef __PRODUCER__
#define __PRODUCER__


#include "common/amq_producer.h"
#include "common/iegad_singleton.hpp"
#include <thread>


class Producer {
public:
    typedef iegad::tools::singleton<iegad::cms_ex::AMQ_Producer> producer_t;
    void Run(int times) {
	producer_t::instance()->SetEnvironment("tcp://192.168.1.101:61616", 0, "testQueue.foo", "system", "1234", 3000, 5000);
	if (!producer_t::instance()->Open()) {
	    std::cout << "Open failed" << std::endl;
	    return;
	}
	for (int i = 0; i < times; i++) {
	    if (!producer_t::instance()->SendTextMessage("出师未捷身先死,长使英雄泪满襟.")) {
		std::cout << "SendTextMessage Failed" << std::endl;
	    }
	}
    }
}; // class Producer;


#endif // __PRODUCER__