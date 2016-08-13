#include "net/iegad_msg_type.hpp"
#include <iostream>



#include <gtest/gtest.h>




struct mtest {
    int a;
    char c;
};

TEST(iegadNet, tcp_msg)
{
    EXPECT_EQ(sizeof(iegad::net::tcp_msg_head), 8);
    EXPECT_EQ(sizeof(mtest), 8);
}

