// ==========================================
// @title : timer 第一轮测试
// @by : iegad, 2016-06-18
// @PS : 这个测试更多需要在实际使用中测试, 单元测试很难测出问题.
// ==========================================


#include <gtest/gtest.h>
#include "tools/iegad_timer.hpp"


using namespace iegad::tools;


void
print(int & n)
{
    std::cout << "hello world" << std::endl;
    n++;
}


TEST(timerTesting, unixTime)
{
    int n = 0;
    timer<int> tmr(print, n, 100, 5);
    tmr.start("2016-06-18 20:57:50");
    EXPECT_EQ(5, n);
}
