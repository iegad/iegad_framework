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
    iegad::tools::_LOG log(argv[0]);
    //iegad::net::tcp_mt_svr host("127.0.0.1", 6688);
    //// host.regist_svc(...)
    //host.run();
    //std::this_thread::sleep_for(std::chrono::seconds(3));


    //std::cout << "done" << std::endl;


    //worker_t<int> wkr(que_, callback_proc);
    //wkr.run(1);
    //std::thread t(thread_proc);



    //t.join();
    //wkr.stop();

    iWARN << "test warning\n" << std::endl;
    iERR << "test 111\n" << std::endl;
    iINFO << "test war222ning\n" << std::endl;     

    std::cin.get();
    //host.stop();
    exit(0);
}
