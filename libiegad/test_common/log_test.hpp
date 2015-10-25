#ifndef __LOG_TEST__
#define __LOG_TEST__



#include "common/iegad_log.h"


class log_test {
public:
    explicit log_test(char * argv0) : 
	handle_(argv0) {}

private:
    iegad::tools::_LOG handle_;
};


#endif // __LOG_TEST__