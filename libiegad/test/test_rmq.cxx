#include <iostream>
#include <clocale>

#include <gtest/gtest.h>

#include "rabbitmq_ex/rabbitmq_producter.hpp"
#include "rabbitmq_ex/rabbitmq_queue.hpp"
#include "tools/iegad_log.hpp"


using namespace iegad;


std::string test_queue_d = "test_queue_d";


TEST(rmqExTesting, CreateAndDelete)
{
    rabbitmq_ex::host_option_t hopt;
    hopt.host = "127.0.0.1";
    rabbitmq_ex::session_t session(hopt);
    //  静态函数创建（删除）队列
    EXPECT_TRUE(rabbitmq_ex::queue_t::createQueue(session, "123"));
    EXPECT_TRUE(rabbitmq_ex::queue_t::deleteQueue(session, "123"));

    // 成员函数创建（删除）队列
    rabbitmq_ex::queue_t que(session, "test_queue_d_temp");
    que.setDurable(true);
    que.setAuto_delete(false);
    que.setExclusive(false);
    que.setPassive(false);
    EXPECT_TRUE(que.createQueue());
    EXPECT_TRUE(que.deleteQueue());
}


TEST(rmqExTesting, purgeQueue)
{
    rabbitmq_ex::session_t session;
    rabbitmq_ex::queue_t que(session, "test_queue_d");
    que.setDurable(true);
    que.setAuto_delete(false);
    que.setExclusive(false);
    que.setPassive(false);
    EXPECT_TRUE(que.purgeQueue());

    rabbitmq_ex::queue_t que1(session, "que1");
    que1.setDurable(true);
    que1.setAuto_delete(false);
    que1.setExclusive(false);
    que1.setPassive(false);
    EXPECT_FALSE(que1.purgeQueue());
}



TEST(rmqExTesting, createDurableQueue)
{
    rabbitmq_ex::session_t session;
    rabbitmq_ex::queue_t que(session, test_queue_d);
    que.setDurable(true);
    que.setAuto_delete(false);
    que.setExclusive(false);
    que.setPassive(false);
    EXPECT_TRUE(que.createQueue());
}


TEST(rmqExTesting, MultiQueueWithOneSession)
{
    rabbitmq_ex::session_t session;
    EXPECT_TRUE(rabbitmq_ex::queue_t::createQueue(session, "d1_queue", false, true, false));
    EXPECT_TRUE(rabbitmq_ex::queue_t::createQueue(session, "d2_queue", false, true));
    EXPECT_TRUE(rabbitmq_ex::queue_t::createQueue(session, "d3_queue"));

    rabbitmq_ex::queue_t que1(session, "d1_queue");
    rabbitmq_ex::queue_t que2(session, "d2_queue");
    rabbitmq_ex::queue_t que3(session, "d3_queue");

    EXPECT_TRUE(rabbitmq_ex::producter::push(que1, "Hello world"));
    EXPECT_TRUE(rabbitmq_ex::producter::push(que2, "Hello world"));
    EXPECT_TRUE(rabbitmq_ex::producter::push(que3, "Hello world"));
}





int
main(int argc, char * argv[])
{
    ::tools::_LOG log(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
