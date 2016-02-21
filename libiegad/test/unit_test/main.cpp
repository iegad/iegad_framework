#include "consumer.hpp"
#include "producer.hpp"


#include <thread>
#include <ctime>


void 
producer_func(int maxCount)
{
    std::clock_t started, finished;
    started = std::clock();
    std::cout << "producer_func start\n";
    Producer c;
    c.Run(maxCount);
    std::cout << "producer_func done\n";
    finished = std::clock();
    std::cout << (finished - started) / CLOCKS_PER_SEC << std::endl;
}


void
consumer_func(int maxCount)
{
    std::clock_t started, finished;
    started = std::clock();
    std::cout << "consumer_func start\n";
    Consumer c;
    c.Run(maxCount);
    std::cout << "consumer_func done\n";
    finished = std::clock();
    std::cout << (finished - started) / CLOCKS_PER_SEC << std::endl;
}



int
main(int argc, char * argv[])
{
    iegad::cms_ex::InitActiveMQEnvironment();
    int maxCount = std::stoi(argv[1]);
    //std::thread p(std::bind(producer_func, maxCount));
    //std::thread c(std::bind(consumer_func, maxCount));

    //p.join();
    //c.join();
    //producer_func(maxCount);
    consumer_func(maxCount);

    std::cin.get();
    exit(0);
}