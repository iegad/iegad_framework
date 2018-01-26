// ==========================================
// @title : tools 第一轮测试
// @by : iegad, 2017-01-26
// ==========================================


#include <gtest/gtest.h>
#include <netinet/in.h>
#include <iegad/tools/iegad_endian.hpp>
#include <iegad/tools/iegad_log.hpp>



TEST(toolsTesting, endianTesting)
{
    unsigned short ns = 0x0102;
    unsigned int   ni = 0x01020304;
    unsigned long long nl = 0x0102030405060708;

    EXPECT_EQ(htons(ns), iegad::endian::to_big(ns));
    EXPECT_EQ(htonl(ni), iegad::endian::to_big(ni));
    EXPECT_EQ(htonll(nl), iegad::endian::to_big(nl));
    EXPECT_EQ(ns, iegad::endian::to_little(ns));
    EXPECT_EQ(ni, iegad::endian::to_little(ni));
    EXPECT_EQ(nl, iegad::endian::to_little(nl));
}


TEST(toolsTesting, logTesting)
{
    iINFO<<"test of info"<<std::endl;
    iWARN<<"test of warning"<<std::endl;
    iERR<<"test of err"<<std::endl;
    //iFATAL<<"test if fatal"<<std::endl;
}
