#include <iostream>
#include <gtest/gtest.h>
#include "tools/iegad_log.hpp"



#include "redis_ex/iegad_redis_key.hpp"
#include "redis_ex/iegad_redis_client.hpp"


using namespace iegad;

TEST(redisTesting, Key)
{
    redis::connection conn("127.0.0.1");
    redis::key_t::ptr_t cmd = redis::key_t::Create(conn);
    EXPECT_EQ(0, cmd->del("name"));
    std::string res = cmd->dump("222");
    EXPECT_FALSE(res.empty());
    EXPECT_EQ(1, cmd->exists("222"));
    EXPECT_EQ(0, cmd->expire("111", 100000));
    EXPECT_EQ(0, cmd->expireat("111", 1460000800));
    EXPECT_EQ(0, cmd->pexpire("111", 100000000000));
    EXPECT_EQ(0, cmd->pexpireat("111", 1460000800));
    std::vector<std::string> keyVct = cmd->keys("*");
    EXPECT_TRUE(keyVct.size() > 0);
    EXPECT_EQ(0, cmd->move("111", 6));
}


//TEST(redisTesting, client)
//{
//    std::string errstr;
//    redis::connection conn("127.0.0.1");
//    redis::client c(conn);
//    EXPECT_EQ(c.echo("\"hello\""), "\"hello\"");
//    EXPECT_TRUE(c.ping(&errstr));
//    std::cout<<errstr<<std::endl;
//    EXPECT_TRUE(c.select(3));
//    EXPECT_TRUE(c.select(0));
//}


