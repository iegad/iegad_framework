#ifndef __PUBLISHER__
#define __PUBLISHER__


#include "common/amq_publisher.h"
#include "common/iegad_singleton.hpp"
#include <thread>


class Publisher {
public:
    typedef iegad::tools::singleton<iegad::cms_ex::AMQ_Publisher> publisher_t;
    void Run(int times) {
	publisher_t::instance()->SetClientId("111");
	publisher_t::instance()->SetEnvironment("tcp://192.168.1.101:61616", 0, "testTopics.foo", "system", "1234", 3000, 5000);
	if (!publisher_t::instance()->Open()) {
	    std::cout << "Open failed" << std::endl;
	    return;
	}
	for (int i = 0; i < times; i++) {
	    if (!publisher_t::instance()->SendTextMessage("出师未捷身先死,长使英雄泪满襟.")) {
		std::cout << "SendTextMessage Failed" << std::endl;
	    }
	}
    }
}; // class Publisher;


#endif // __PUBLISHER__