#include "tcp_mt_server.h"
#include "msg_basic.pb.h"
#include "iegad_log.h"
#include "iegad_io_msg.h"


using namespace boost::asio;
using namespace iegad::common;


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
    msg_basic msgbsc;

    for (;;) {
	if (clnt.is_open()) {
	    clnt.close();
	}
	// step 1 : check the stop_flag;
	if (_is_stop()) {
	    break;
	}
	// step 2 : waiting for the connecting;
	if (this->_accept(clnt, err_code) != 0) {
	    continue;
	}

	// step 3 : get the msg_dbstr;
	if (this->_build_msg_basic(clnt, msgbsc, err_code) != 0) {
	    continue;
	}
	// step 4 : call the service;
	this->_call_svc(clnt, msgbsc);
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
iegad::net::tcp_mt_svr::_is_stop()
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

    // 设置超时
    if (setsockopt(clnt.native(), SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout_, sizeof(int)) != 0) {
	return -1;
    }

    // 关闭 NAGLE 算法
    clnt.set_option(ip::tcp::no_delay(true));
    
    return 0;
}


void 
iegad::net::tcp_mt_svr::regist_svc(svc_basic_ptr svc_obj)
{
    if (iegad::net::svc_basic::regist_svc(svc_obj, svc_map_) != 0) {
	iERR << "### the service object already have ###" << std::endl;
    }
}


int
iegad::net::tcp_mt_svr::_build_msg_basic(ip::tcp::socket & clnt,
iegad::net::msg_basic & msgbsc,
boost::system::error_code & err_code)
{
    int rzt;
    rzt = iegad::net::recv_msg_basic(clnt, msgbsc, err_code);
    if (err_code == boost::asio::error::timed_out) {
	iWARN << clnt.remote_endpoint().address().to_string() << " ### TIME OUT ###" << std::endl;
    }
    return rzt;
}


int 
iegad::net::tcp_mt_svr::_call_svc(ip::tcp::socket & clnt, 
						  iegad::net::msg_basic & msgbsc)
{
    svc_basic_ptr p_svc = svc_basic::get_svc(msgbsc.msg_type(), svc_map_);
    if (p_svc.get() != nullptr) {
    //msg_type mapping the msg_svc;
	if (p_svc->action(clnt, msgbsc.msg_flag(), msgbsc.msg_bdstr())) {
	    iWARN << p_svc->get_id() << "'s service called failed" << std::endl;
	}
    }  // if (itor != svc_map_.end())
    else {
    //msg_type don't mapping the msg_svc;
	iERR << "### no service mapping ###" << std::endl;
	return -1;
    }
    clnt.close();
    return 0;
}


iegad::net::tcp_mt_svr::~tcp_mt_svr()
{
    this->stop();
}
