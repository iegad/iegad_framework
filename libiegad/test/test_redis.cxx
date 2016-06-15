#include <iostream>
#include <gtest/gtest.h>
#include "tools/iegad_log.hpp"



#include "redis_ex/iegad_redis_key.hpp"
#include "redis_ex/iegad_redis_client.hpp"


using namespace iegad;

TEST(redisTesting, connection)
{
    std::string errstr;
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
    EXPECT_EQ("abcd", keys[0]);
    EXPECT_EQ("name", keys[1]);
    EXPECT_EQ("iegad", keys[2]);
    EXPECT_FALSE(key.rename("111", "222", &errstr));
    //EXPECT_TRUE(key.renamenx("iegad", "iegadd")); 该函数api有问题
    EXPECT_TRUE(key.rename("iegad", "iegadd"));
    EXPECT_TRUE(key.rename("iegadd", "iegad"));
}


TEST(redisTesting, client)
{
    std::string errstr;
    redis::connection conn("127.0.0.1");
    redis::client c(conn);
    EXPECT_EQ(c.echo("\"hello\""), "\"hello\"");
    EXPECT_TRUE(c.ping(&errstr));
    std::cout<<errstr<<std::endl;
    EXPECT_TRUE(c.select(3));
    EXPECT_TRUE(c.select(0));
}

int
main(int argc, char * argv[])
{
    ::tools::_LOG log(argv[0]);

    //for (int i = 0; i < 3; i++) {
    //    std::thread p(pf);
    //    p.detach();
    //    std::thread c(cf);
    //    c.detach();
    //}
    //
    //std::this_thread::sleep_for(std::chrono::seconds(20));

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
