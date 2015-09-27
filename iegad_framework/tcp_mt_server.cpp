#include "tcp_mt_server.h"
#include "msg_basic.pb.h"
#include "iegad_log.hpp"


using namespace boost::asio;
using namespace iegad::tools;


iegad::net::tcp_mt_svr::tcp_mt_svr(
    const std::string & host, 
    unsigned short port)
    :
    stop_flag_(false),
    ios_(),
    acptor_(ios_, 
    ip::tcp::endpoint(ip::address::from_string(host), port)) 
{//ctor
    // set SO_REUSEADDR on
    acptor_.set_option(ip::tcp::acceptor::reuse_address(true));
    // set TCP_NAGLE off
    acptor_.set_option(ip::tcp::no_delay(true));
}


void 
iegad::net::tcp_mt_svr::_thread_proc()
{// thread proc : master thread of server's service.    
    io_service ios;
    ip::tcp::socket clnt(ios);
    boost::system::error_code err_code;
    std::string msg_basic_str;

    for (;;) {
	clnt.close();
	// step 1 : check the stop_flag;
	if (get_stop()) {
	    break;
	}
	// step 2 : waiting for the connecting;
	if (this->_accept(clnt, err_code) != 0) {
	    continue;
	}

	// step 3 : get the msg_dbstr;
	;
	if (msg_basic_str = this->_get_svcbd_str(clnt, err_code), 
	    msg_basic_str == "") {
	    continue;
	}
	// step 4 : call the service;
	this->_call_svc(clnt, msg_basic_str);
    } // for (;;);
}


void 
iegad::net::tcp_mt_svr::run(int n /*= 8*/)
{// run the service
    iINFO << "@@@ threads num : " << n << std::endl;
    for (int i = 0; i < n; i++) {
	thread_pool_.push_back(
	    std::thread(std::bind(&tcp_mt_svr::_thread_proc, this)));
    }
}


void 
iegad::net::tcp_mt_svr::stop()
{// stop the service
    if (stop_flag_) {
	return;
    }
    mtx_lock_t locker(stop_mtx_);
    stop_flag_ = true;
    locker.unlock();
    acptor_.close();    
    for (int i = 0, n = thread_pool_.size(); i < n; i++) {
	thread_pool_[i].join();
    }
    iINFO << "@@@ service's stopped @@@" << std::endl;
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
    locker.unlock();
    if (err_code) {
	if (!stop_flag_) {
	    // Q & A : 这里必需用两层判断, 不然线程在join的时候会出现错误!
	    iERR << err_code.message() << std::endl;
	}
	return -1;
    }
    clnt.set_option(ip::tcp::no_delay(true));
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
	iERR << "### the service object already have ###" << std::endl;
    }
}


const std::string 
iegad::net::tcp_mt_svr::_get_svcbd_str(ip::tcp::socket & clnt, boost::system::error_code & err_code)
{
    char read_buf[BUF_SIZE];
    std::string rzt;
    int nbytes = 0;
    do
    {
	nbytes = clnt.read_some(buffer(read_buf), err_code);
	if (err_code) {
	    if (err_code != boost::asio::error::eof) {
		iERR << "clnt.read_some | " << err_code.message() << std::endl;
		rzt = "";
		break;
	    }
	    iINFO << "clnt.read_some | eof : " << err_code.message() << std::endl;
	    break;
	}
	else {
	    rzt.append(read_buf, nbytes);
	}
    } while (!err_code && nbytes > 0);
    return  rzt;
}


int 
iegad::net::tcp_mt_svr::_call_svc(ip::tcp::socket & clnt, std::string & msg_basic_str)
{
    tcp_msg msg;
    if (msg.ParseFromString(msg_basic_str)) {
	//msg builded sucess;
	svc_t::iterator itor = svc_map_.find(msg.msg_type());

	if (itor != svc_map_.end()) {
	    //msg_type mapping the msg_svc;
	    itor->second->action(clnt, msg.msg_flag(), msg.msg_bdstr());
	}
	// if (itor != svc_map_.end())
	else {
	    //msg_type don't mapping the msg_svc;
	    iERR << "### no service mapping ###" << std::endl;
	    return -1;
	}
    } // if (msg.ParseFromString(bdstr));
    else {
	//msg builed failed;
	iERR << "### msg.ParseFromString(msg_basic_str) failed ###" << std::endl;
	return -1;
    }
    clnt.close();
    return 0;
}
