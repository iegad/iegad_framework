#ifndef __CONSUMER__
#define __CONSUMER__


#include "common/amq_consumer.h"
#include "common/iegad_singleton.hpp"


class Consumer {
public:
    typedef iegad::tools::singleton<iegad::cms_ex::AMQ_Consumer> consumer_t;
    void Run(int times) {
	std::string str;
	consumer_t::instance()->SetEnvironment("tcp://192.168.1.101:61616", 0, "testQueue.foo", "system", "1234", 3000, 5000);
	if (!consumer_t::instance()->Open()) {
	    std::cout << "Open failed" << std::endl;
	    return;
	}
	for (int i = 0; i < times; i++) {
	    if (consumer_t::instance()->RecvTextMessage(&str)) {
		std::cout << str << std::endl;
	    }
	}
    }
}; // class Consumer;



#endif // __CONSUMER__