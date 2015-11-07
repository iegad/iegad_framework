#ifndef __TIMER_TEST__
#define __TIMER_TEST__


#include "common/iegad_timer.hpp"
#include <iostream>


void
timer_fun(std::string & str)
{
    std::cout << "[ timer test ] : " << str << std::endl;
}


void
timer_test(const std::string & timestr = "") 
{
    if (timestr == "") {
	std::string tmr1_param = "111222333444555";
	iegad::tools::timer<std::string> tmr1(timer_fun, tmr1_param, 1000, 10);
	tmr1.start();
    }
    else {
	std::string tmr2_param = "aaabbbcccdddeee";
	iegad::tools::timer<std::string> tmr2(timer_fun, tmr2_param, timestr, 1, 5);
	tmr2.start();
    }
}


#endif // __TIMER_TEST__