#include <boost/asio.hpp>


#include <iostream>
#include "tcp_mt_server.h"
#include "job_worker.hpp"
#include "iegad_log.hpp"
#include "echo_svc.h"



using namespace iegad::tools;



int 
main(int argc, char * argv[])
{
    std::shared_ptr<iegad::net::echo_svc> echo_svc_(new iegad::net::echo_svc(10));
    std::cout << echo_svc_->get_id();
    iegad::tools::_LOG log(argv[0]);
    iegad::net::tcp_mt_svr host("0.0.0.0", 6688);
    host.regist_svc(echo_svc_);
    host.run(1);
    std::this_thread::sleep_for(std::chrono::seconds(3));




    std::cin.get();
    std::cout << "done" << std::endl;
    host.stop();
    exit(0);
}
