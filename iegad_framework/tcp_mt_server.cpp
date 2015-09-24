#include "tcp_mt_server.h"
#include "msg_basic.pb.h"
//#include "logger.h"


using namespace boost::asio;
//using namespace iegad::tools;


iegad::net::tcp_mt_svr::tcp_mt_svr(
    const std::string & host, 
    unsigned short port)
    :
    stop_flag_(false),
    ios_(),
    acptor_(ios_, 
    ip::tcp::endpoint(ip::address::from_string(host), port)) 
{//ctor
    acptor_.set_option(ip::tcp::acceptor::reuse_address(true));
}


void 
iegad::net::tcp_mt_svr::_thread_proc()
{// thread proc : master thread of server's service.
    io_service ios;
    int nbytes;
    ip::tcp::socket clnt(ios);
    boost::system::error_code err_code;
    char read_buf[BUF_SIZE];
    auto buff = buffer(read_buf);
    tcp_msg msg;


    for (;;) {
	// step 1 : check the stop_flag;
	if (get_stop()) {

	    break;
	}
	// step 2 : waiting for the connecting;
	if (this->_accept(clnt, err_code) != 0) {
	    continue;
	}
	// step 3 : read buffer from client;
	nbytes = 0;
	do 
	{
	    nbytes += clnt.read_some(buff, err_code);
	    if (err_code) {
		if (err_code != boost::asio::error::eof) {

		}
		break;
	    }
	} while (err_code && nbytes > 0);
	read_buf[nbytes] = 0;


	// step 4 : transfer the buffer to the msg;
	if (msg.ParseFromString(read_buf)) {
	    // step 5 : find the service;
	    svc_t::iterator itor = svc_map_.find(msg.msg_type());
	    // step 6 : call the service's action;
	    if (itor != svc_map_.end()) {
		if (itor->second->action(msg.msg_flag(), msg.msg_bdstr()) != 0) {

		}
	    } // if (itor != svc_map_.end())
	    else {

	    }

	} // if (msg.ParseFromString(read_buf));
	else {

	}
	clnt.close();
    } // for (;;);
}


void 
iegad::net::tcp_mt_svr::run(int n /*= 8*/)
{// run the service
    for (int i = 0; i < n; i++) {
	
	thread_pool_.push_back(
	    std::thread(std::bind(&tcp_mt_svr::_thread_proc, this)));
    }
}


void 
iegad::net::tcp_mt_svr::stop()
{// stop the service
    mtx_lock_t locker(stop_mtx_);
    stop_flag_ = true;
    locker.unlock();
    acptor_.close();
    
    for (int i = 0, n = thread_pool_.size(); i < n; i++) {
	thread_pool_[i].join();
    }
    
}


bool 
iegad::net::tcp_mt_svr::get_stop()
{// test the stop_flag_
    mtx_lock_t locker(stop_mtx_);
    return stop_flag_;
}


int 
iegad::net::tcp_mt_svr::_accept(ip::tcp::socket & clnt, boost::system::error_code & err_code)
{
    mtx_lock_t locker(thread_mtx_);
    acptor_.accept(clnt, err_code);
    if (err_code) {
	
	return -1;
    }
    return 0;
}


void 
iegad::net::tcp_mt_svr::regist_svc(svc_basic_ptr svc_obj)
{
    svc_t::iterator itor = svc_map_.find(svc_obj->get_id());
    if (itor == svc_map_.end()) {
	svc_map_.insert(std::pair<int, svc_basic_ptr>(svc_obj->get_id(), svc_obj));
    }
    else {
	
    }
}
