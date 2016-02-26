#include "consumer.hpp"
#include "producer.hpp"
#include "subscriber.hpp"
#include "publisher.hpp"


#include <thread>
#include <ctime>


#include <iostream>


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


void
publisher_func(int maxCount)
{
    std::clock_t started, finished;
    started = std::clock();
    std::cout << "producer_func start\n";
    Publisher c;
    c.Run(maxCount);
    std::cout << "producer_func done\n";
    finished = std::clock();
    std::cout << (finished - started) / CLOCKS_PER_SEC << std::endl;
}


void
subscriber_func(int maxCount)
{
    std::clock_t started, finished;
    started = std::clock();
    std::cout << "consumer_func start\n";
    Subscriber c;
    c.Run(maxCount);
    std::cout << "consumer_func done\n";
    finished = std::clock();
    std::cout << (finished - started) / CLOCKS_PER_SEC << std::endl;
}


#include "common/Astroboy.svr.hpp"


bool InitEnvironment() {
    return true;
}


class EchoImp : public iegad::thrift_ex::IElefunImp {
public:
    EchoImp(int32_t funno) 
	: iegad::thrift_ex::IElefunImp(funno) {}


    virtual int32_t _action(const std::string & msgstr, int32_t msgsize, 
	std::string * resstr, int32_t * ressize, 
	int32_t * errcode, std::string * errstr) {
	*resstr = msgstr;
	return 0;
    }
}; // class EchoImp;


int
main(int argc, char * argv[])
{
    //iegad::cms_ex::InitActiveMQEnvironment();
    //int maxCount = std::stoi(argv[1]);
    //std::thread p(std::bind(publisher_func, maxCount));
    //std::thread c(std::bind(subscriber_func, maxCount));

    //p.join();
    //c.join();
    using iegad::thrift_ex::Astroboy_svr;
    using iegad::thrift_ex::AstroboyHandler;
    do {
	if (!Astroboy_svr::InitEnvironment(InitEnvironment)) {
	    break;
	}
	std::shared_ptr<Astroboy_svr::action_map_t> map(new Astroboy_svr::action_map_t);
	std::shared_ptr<EchoImp> echoImp(new EchoImp(1));
	map->insert(std::make_pair(echoImp->No(), echoImp));
	AstroboyHandler::SetActionMap(map);
	Astroboy_svr astro(6688);
	astro.Run();
    } while (false);


    std::cin.get();
    exit(0);
}