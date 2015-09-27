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
    //iegad::tools::_LOG log(argv[0]);
    //std::shared_ptr<iegad::net::echo_svc> echo_svc_(new iegad::net::echo_svc(10));
    //std::cout << echo_svc_->get_id();


    //iegad::net::tcp_mt_svr host("127.0.0.1", 6688);
    //host.regist_svc(echo_svc_);
    //host.run(1);

    //std::cin.get();

    std::cout << sizeof(bool) << std::endl;
    std::cout << sizeof(std::mutex) << std::endl;
    std::cout << sizeof(boost::asio::io_service) << std::endl;
    std::cout << sizeof(iegad::net::tcp_mt_svr::thread_pool_t) << std::endl;
    std::cout << sizeof(iegad::net::tcp_mt_svr::svc_t) << std::endl;
    std::cout << sizeof(boost::asio::ip::tcp::acceptor) << std::endl;

    std::cout << "done" << std::endl;
    std::cin.get();
    exit(0);
}
