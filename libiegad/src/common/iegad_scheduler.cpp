#include "iegad_scheduler.h"
#include <cassert>


void 
iegad::tools::scheduler::run()
{
    stop_flag_ = true;
    for (;;) {
	if (stop_flag_) {
	    break;
	}
	func_();
	std::this_thread::sleep_for(std::chrono::milliseconds(ms_));
    }
}


iegad::tools::scheduler::scheduler(func_t func, int ms)
    : stop_flag_(false), ms_(ms), func_(func) 
{
    assert(ms > 0);
}


void 
iegad::tools::scheduler::stop()
{
    stop_flag_ = true;
}

