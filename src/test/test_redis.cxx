#include <iostream>
#include <gtest/gtest.h>
#include "tools/iegad_log.hpp"


#include "redis_ex/iegad_redis_key.hpp"


using namespace iegad;

TEST(redisTesting, connection)
{
    redis::connection conn("127.0.0.1");
    redis::key_t key(conn);
    EXPECT_EQ(0, key.exists("testKey"));
    EXPECT_EQ(1, key.exists("iegad"));
    EXPECT_EQ(0, key.del("test"));
    std::cout << "dump key : " << key.dump("iegad").size() << std::endl;
    EXPECT_EQ(1, key.expire("iegad", 60));
    EXPECT_EQ(0, key.expire("ieddad", 100));
    EXPECT_EQ(1, key.persist("iegad"));
    auto keys = key.keys("*");
    EXPECT_EQ(3, keys.size());
    EXPECT_EQ("iegad", keys[0]);
    EXPECT_EQ("abcd", keys[1]);
    EXPECT_EQ("name", keys[2]);
    EXPECT_FALSE(key.rename("111", "222"));
    EXPECT_TRUE(key.renamenx("iegad", "iegadd"));
    EXPECT_TRUE(key.rename("iegadd", "iegad"));
}
