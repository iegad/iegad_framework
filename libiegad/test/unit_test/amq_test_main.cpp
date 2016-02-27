//#include "consumer.hpp"
//#include "producer.hpp"
//#include "subscriber.hpp"
//#include "publisher.hpp"
//#include <thread>
//#include <ctime>
//#include <iostream>
//
//
//void 
//producer_func(int maxCount)
//{
//    std::clock_t started, finished;
//    started = std::clock();
//    std::cout << "producer_func start\n";
//    Producer c;
//    c.Run(maxCount);
//    std::cout << "producer_func done\n";
//    finished = std::clock();
//    std::cout << (finished - started) / CLOCKS_PER_SEC << std::endl;
//}
//
//
//void
//consumer_func(int maxCount)
//{
//    std::clock_t started, finished;
//    started = std::clock();
//    std::cout << "consumer_func start\n";
//    Consumer c;
//    c.Run(maxCount);
//    std::cout << "consumer_func done\n";
//    finished = std::clock();
//    std::cout << (finished - started) / CLOCKS_PER_SEC << std::endl;
//}
//
//
//void
//publisher_func(int maxCount)
//{
//    std::clock_t started, finished;
//    started = std::clock();
//    std::cout << "producer_func start\n";
//    Publisher c;
//    c.Run(maxCount);
//    std::cout << "producer_func done\n";
//    finished = std::clock();
//    std::cout << (finished - started) / CLOCKS_PER_SEC << std::endl;
//}
//
//
//void
//subscriber_func(int maxCount)
//{
//    std::clock_t started, finished;
//    started = std::clock();
//    std::cout << "consumer_func start\n";
//    Subscriber c;
//    c.Run(maxCount);
//    std::cout << "consumer_func done\n";
//    finished = std::clock();
//    std::cout << (finished - started) / CLOCKS_PER_SEC << std::endl;
//}
//
//
//int
//main(int argc, char * argv[])
//{
//    iegad::cms_ex::InitActiveMQEnvironment();
//    int maxCount = std::stoi(argv[1]);
//    std::thread p(std::bind(publisher_func, maxCount));
//    std::thread c(std::bind(subscriber_func, maxCount));
//
//    p.join();
//    c.join();
//    
//    std::cin.get();
//    exit(0);
//}