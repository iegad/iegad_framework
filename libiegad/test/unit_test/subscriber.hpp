#ifndef __SUBSCRIBER__
#define __SUBSCRIBER__


#include "common/amq_subscriber.h"
#include "common/iegad_singleton.hpp"


class Subscriber {
public:
    typedef iegad::tools::singleton<iegad::cms_ex::AMQ_Subscriber> subscriber_t;
    void Run(int times) {
	std::string str;
	subscriber_t::instance()->SetClientId("111");
	subscriber_t::instance()->SetEnvironment("tcp://192.168.1.101:61616", 0, "testTopics.foo", "system", "1234", 3000, 5000);
	if (!subscriber_t::instance()->Open(&str)) {
	    std::cout << "Open failed" << std::endl;
	    return;
	}
	for (int i = 0; i < times; i++) {	    
	    if (subscriber_t::instance()->RecvTextMessage(&str)) {
		std::cout << str << std::endl;
	    }
	}
    }
}; // class Subscriber;



#endif // __CONSUMER__