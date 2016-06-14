#include <iostream>
#include <gtest/gtest.h>


#include "rabbitmq_ex/rabbitmq_queue.hpp"
#include "rabbitmq_ex/rabbitmq_exchange.hpp"
#include "rabbitmq_ex/rabbitmq_producter.hpp"
#include "rabbitmq_ex/rabbitmq_consumer.hpp"
#include "rabbitmq_ex/rabbitmq_publisher.hpp"
#include "rabbitmq_ex/rabbitmq_subcriber.hpp"
#include "tools/iegad_log.hpp"


using namespace iegad;


std::string test_queue_d = "test_queue_d";
std::string test_exchange_d = "test_exchange_d";



TEST(rmqExTesting, queueTesting)
{// 队列测试
    rabbitmq_ex::host_option_t hopt;
    hopt.host = "127.0.0.1";
    hopt.usr = "iegad";
    hopt.pwd = "1111";
    rabbitmq_ex::session_t session(hopt);

    //  静态函数创建（删除）队列
    EXPECT_TRUE(rabbitmq_ex::queue_t::createQueue(session, "123"));
    EXPECT_TRUE(rabbitmq_ex::queue_t::purgeQueue(session, "123"));
    EXPECT_TRUE(rabbitmq_ex::queue_t::deleteQueue(session, "123"));

    // 成员函数创建（删除）队列
    rabbitmq_ex::queue_t que(session, "test_queue_d_temp");
    que.setDurable(true);
    que.setAuto_delete(false);
    que.setExclusive(false);
    que.setPassive(false);
    EXPECT_TRUE(que.createQueue());
    EXPECT_TRUE(que.purgeQueue());
    EXPECT_TRUE(que.deleteQueue());

    // 创建执久化队列
    que.setName(test_queue_d);
    EXPECT_EQ(test_queue_d, que.name());
    EXPECT_TRUE(que.durable());
    EXPECT_FALSE(que.passive());
    EXPECT_FALSE(que.auto_delete());
    EXPECT_FALSE(que.exclusive());
    EXPECT_TRUE(que.createQueue());

    // 测试会话信息
    EXPECT_EQ("127.0.0.1", que.session().host());
    EXPECT_EQ(5672, que.session().port());
    EXPECT_EQ("iegad", que.session().usr());
    EXPECT_EQ("1111", que.session().pwd());
    EXPECT_EQ("/", que.session().vhost());
}


TEST(rmqExTesting, exchangeTesting)
{// 交换器测试
    rabbitmq_ex::session_t ss;

    // 静态函数创建（删除）交换器
    EXPECT_TRUE(rabbitmq_ex::exchange_t::createExchange(ss, "tempExchange",
                                            rabbitmq_ex::exchange_t::EXCHANGE_TYPE_FANOUT, false, false, true));
    EXPECT_TRUE(rabbitmq_ex::exchange_t::deleteExchange(ss, "tempExchange"));

    rabbitmq_ex::exchange_t exc(ss, test_exchange_d,
                                rabbitmq_ex::exchange_t::EXCHANGE_TYPE_FANOUT);

    // 成员函数创建（删除）执久化交换器
    exc.setDurable(true);
    EXPECT_TRUE(exc.durable());
    EXPECT_FALSE(exc.auto_delete());
    EXPECT_FALSE(exc.passive());
    EXPECT_EQ(test_exchange_d, exc.name());
    EXPECT_EQ(rabbitmq_ex::exchange_t::EXCHANGE_TYPE_FANOUT, exc.type());
    EXPECT_TRUE(exc.createExchange());
    EXPECT_TRUE(exc.deleteExchange());
}


bool
rcvHandler(rabbitmq_ex::consumer::Envelope_ptr env, const std::string & errstr)
{// 接收句柄
    if (env == nullptr || !errstr.empty()) {
        iERR << errstr << std::endl;
        return false;
    }
    iINFO << std::this_thread::get_id() << std::endl << env->Message()->Body() << std::endl;
    return true;
}



TEST(rmqExTesting, ProducterAndConsumer)
{// 生产者－消息者
    rabbitmq_ex::session_t ss;
    rabbitmq_ex::queue_t que(ss, test_queue_d);

    rabbitmq_ex::producter p(que);
    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(p.send(std::to_string(i) + " hello world"));
    }

    rabbitmq_ex::consumer c(que.name(), que);
    for (int i = 0; i < 100; i++) {
        std::string msgstr;
        std::string errstr;
        EXPECT_TRUE(c.recv(&msgstr, true, -1, &errstr));
        EXPECT_EQ(std::to_string(i) + " hello world", msgstr);
    }
}


void pf() {
    rabbitmq_ex::session_t ss;
    rabbitmq_ex::queue_t que(ss, test_queue_d);

    rabbitmq_ex::producter p(que);
    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(p.send(std::to_string(i) + " hello world"));
    }
}


void cf() {
    rabbitmq_ex::session_t ss;
    rabbitmq_ex::queue_t que(ss, test_queue_d);

    rabbitmq_ex::consumer c("", que);
    EXPECT_TRUE(c.beginRecvMessage(rcvHandler, false));
    std::this_thread::sleep_for(std::chrono::seconds(5));
    c.endRecvMessage();
}


void subfun() {
    rabbitmq_ex::session_t ss;
    rabbitmq_ex::exchange_t exc(ss, test_exchange_d,
                                rabbitmq_ex::exchange_t::EXCHANGE_TYPE_FANOUT);
    rabbitmq_ex::subcirber sub(exc, std::to_string(std::rand()));
    std::string msgstr;
    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(sub.recv(&msgstr));
        EXPECT_EQ(std::to_string(i) + " hello world", msgstr);
        iINFO << std::this_thread::get_id() << msgstr << std::endl;
    }
}


TEST(rmqExTesting, PublisherAndSubcriber)
{// 生产者－消息者
    rabbitmq_ex::session_t ss;
    rabbitmq_ex::exchange_t exc(ss, test_exchange_d,
                                rabbitmq_ex::exchange_t::EXCHANGE_TYPE_FANOUT);
    EXPECT_TRUE(exc.createExchange());
    rabbitmq_ex::publisher p(exc);
    std::string errstr;
    std::thread t1(subfun);
    t1.detach();
    std::thread t2(subfun);
    t2.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    for (int i = 0; i < 100; i++) {
        EXPECT_TRUE(p.send(std::to_string(i) + " hello world", &errstr));
        if (errstr != "") {
            iINFO << errstr << std::endl;
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    EXPECT_TRUE(exc.deleteExchange());
}


void subfund() {
    rabbitmq_ex::session_t ss;
    rabbitmq_ex::exchange_t exc(ss, "test_exchange_direct",
                                rabbitmq_ex::exchange_t::EXCHANGE_TYPE_DIRECT);
    rabbitmq_ex::subcirber sub(exc, std::to_string(std::rand()), "1");
    std::string msgstr;
    for (int i = 0; i < 100; i += 2) {
        EXPECT_TRUE(sub.recv(&msgstr));
        EXPECT_EQ(std::to_string(i) + " hello world", msgstr);
        iINFO << std::this_thread::get_id() << msgstr << std::endl;
    }
}



TEST(rmqExTesting, PublisherDirectAndSubcriberDirect)
{// 生产者－消息者
    rabbitmq_ex::session_t ss;
    rabbitmq_ex::exchange_t exc(ss, "test_exchange_direct",
                                rabbitmq_ex::exchange_t::EXCHANGE_TYPE_DIRECT);
    EXPECT_TRUE(exc.createExchange());
    rabbitmq_ex::publisher p(exc);
    std::string errstr;
    std::thread t1(subfund);
    t1.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    for (int i = 0; i < 100; i ++) {
        if (i % 2 == 0) {
            p.setRoutingKey("1");
        }
        else {
            p.setRoutingKey("2");
        }
        EXPECT_TRUE(p.send(std::to_string(i) + " hello world", &errstr));
        if (errstr != "") {
            iINFO << errstr << std::endl;
        }
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    EXPECT_TRUE(exc.deleteExchange());
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
