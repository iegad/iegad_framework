#include "tcp_mt_svr.h"
#include "msg/basic_msg.pb.h"
#include "common/iegad_log.h"
#include "msg/iegad_io_msg.h"
#include "common/iegad_string.h"


using namespace boost::asio;
using namespace iegad::common;


iegad::net::tcp_mt_svr::tcp_mt_svr(
    unsigned short port)
    :
    stop_flag_(false),
    ios_(),
    acptor_(ios_, ip::tcp::endpoint(ip::tcp::v4(), port), true)
{//ctor
    // set TCP_NAGLE off
    acptor_.set_option(ip::tcp::no_delay(true));
}


iegad::net::tcp_mt_svr::tcp_mt_svr(
    const std::string & host, 
    const std::string & svc)
    :
    stop_flag_(false),
    ios_(),
    acptor_(ios_)
{
    bool binded = false;
    acptor_.open(boost::asio::ip::tcp::v4());

    boost::asio::ip::tcp::resolver::iterator end;
    boost::system::error_code errcode = boost::asio::error::bad_descriptor;

    boost::asio::ip::tcp::resolver rlv(acptor_.get_io_service());
    boost::asio::ip::tcp::resolver::query qry(host, svc);

    boost::asio::ip::tcp::resolver::iterator iter = rlv.resolve(qry);
    for (; errcode && iter != end; ++iter) {
	if (acptor_.bind(*iter, errcode) == 0) {
	    binded = true;
	    break;
	}
    } // for (; errcode && iter != end; ++iter);
    assert(binded);
    acptor_.set_option(ip::tcp::socket::reuse_address(true), errcode);
    acptor_.listen();
}


void 
iegad::net::tcp_mt_svr::_thread_proc()
{// thread proc : master thread of server's service.    
    io_service ios;
    ip::tcp::socket clnt(ios);
    boost::system::error_code err_code;
    basic_msg msgbsc;

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
	if (this->_build_basic_msg(clnt, msgbsc, err_code) != 0) {
	    continue;
	}
	// step 4 : call the service;
	this->_call_svc(clnt, msgbsc);
    } // for (;;);
}


void 
iegad::net::tcp_mt_svr::run(int n /*= 8*/)
{// run the service
    assert(n > 0);
    iINFO << "@@@ threads num : " << n << " @@@"<<std::endl;
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
iegad::net::tcp_mt_svr::regist_svc(basic_svc_ptr svc_obj)
{
    if (iegad::net::basic_svc::regist_svc(svc_obj, svc_map_) != 0) {
	iERR << "### the service object already have ###" << std::endl;
    }
}


int
iegad::net::tcp_mt_svr::_build_basic_msg(ip::tcp::socket & clnt,
iegad::net::basic_msg & msgbsc,
boost::system::error_code & err_code)
{
    int rzt;
    rzt = iegad::net::recv_basic_msg(clnt, msgbsc, err_code);
    if (err_code == boost::asio::error::timed_out) {
	iWARN << clnt.remote_endpoint().address().to_string() << " ### TIME OUT ###" << std::endl;
    }
    return rzt;
}


int 
iegad::net::tcp_mt_svr::_call_svc(ip::tcp::socket & clnt, 
						  iegad::net::basic_msg & msgbsc)
{
    basic_svc_ptr p_svc = basic_svc::get_svc(msgbsc.msg_type(), svc_map_);
    if (p_svc.get() != nullptr) {
	p_svc->action(clnt, msgbsc.msg_flag(), msgbsc.msg_bdstr());
    } 
    else {
    //msg_type don't mapping the msg_svc;
	iWARN << "### no service mapping ###" << std::endl;
	return -1;
    }
    clnt.close();
    return 0;
}


iegad::net::tcp_mt_svr::~tcp_mt_svr()
{
    this->stop();
}


const std::string 
iegad::net::tcp_mt_svr::host_endpoint()
{
    return acptor_.local_endpoint().address().to_string() + ":" + iegad::string::to_str(acptor_.local_endpoint().port());
}
