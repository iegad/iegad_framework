#include <iostream>
#include <gtest/gtest.h>


#include "rabbit_ex/iegad_rabbit_handler.hpp"



using namespace iegad;


TEST(rmqTesting, publishMsg) {
    boost::asio::io_service ioService;
    rabbit_ex::asioHandler handler(ioService);
    handler.connect("localhost", 5672);

    AMQP::Connection connection(&handler, AMQP::Login("guest", "guest"), "/");
    AMQP::Channel channel(&connection);
    EXPECT_TRUE(channel.declareQueue("test_queue_d"));
    std::cout<<channel.connected()<<std::endl;
    AMQP::Deferred & d = channel.startTransaction();
    d.onSuccess([](){ std::cout<<"transaction commit!!!\n"; });
    d.onError([&](const char * errstr) {
        std::cout<<"transaction faild : "<<errstr<<std::endl;
        channel.rollbackTransaction(); });

    for (int i = 0; i < 1000; i++) {
        EXPECT_TRUE(channel.publish("", "test_queue_d", "Hello World!"));
    }
    EXPECT_TRUE(channel.commitTransaction());
    ioService.run();
}


