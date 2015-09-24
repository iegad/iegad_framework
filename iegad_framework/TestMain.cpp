#include <boost/asio.hpp>


#include <iostream>
#include "tcp_mt_server.h"
#include "job_worker.hpp"
#include "iegad_log.hpp"



using namespace iegad::tools;


job_que_t<int> que_;


int
callback_proc(int & a)
{
    std::cout << a << std::endl;
    return 0;
}


void 
thread_proc() {
    for (int i = 0; i < 10; i++) {
	std::this_thread::sleep_for(std::chrono::seconds(1));
	que_.push(i);
    }
}



int 
main(int argc, char * argv[])
{
    //iegad::tools::log::init("2015-09-20.log");
    //iegad::net::tcp_mt_svr host("127.0.0.1", 6688);
    //// host.regist_svc(...)
    //host.run();
    //std::this_thread::sleep_for(std::chrono::seconds(3));
    //host.stop();

    //std::cout << "done" << std::endl;


    //worker_t<int> wkr(que_, callback_proc);
    //wkr.run(1);
    //std::thread t(thread_proc);



    //t.join();
    //wkr.stop();

    //std::cout << "done" << std::endl;

    iegad::tools::_LOG log(argv[0]);

    iINFO << "TEST" << std::endl;
    iWARN << "test" << std::endl;
    iERR << "test" << std::endl;

    std::cout << "done" << std::endl;
    std::cin.get();

    exit(0);
}
