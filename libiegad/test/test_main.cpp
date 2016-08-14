#include <iostream>
#include <clocale>
#include <locale>
#include <gtest/gtest.h>
#include "rabbit_ex/iegad_rabbit_handler.hpp"

using namespace iegad;


int
main(int argc, char * argv[])
{
    std::setlocale(LC_ALL, "zh_CN.UTF-8");
    testing::InitGoogleTest(&argc, argv);

    boost::asio::io_service ioService;
    rabbit_ex::asioHandler handler(ioService);
    handler.connect("localhost", 5672);

    AMQP::Connection connection(&handler, AMQP::Login("guest", "guest"), "/");
    AMQP::Channel channel(&connection);
    std::cout<<channel.connected()<<std::endl;
    channel.publish("", "test_queue_d", "Hello World!");
    ioService.run();
    return 0;//RUN_ALL_TESTS();
}

