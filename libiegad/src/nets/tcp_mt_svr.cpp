#include "tcp_mt_svr.h"
#include "msg/basic_msg.pb.h"
#include "common/iegad_log.h"
#include "msg/iegad_msg.h"
#include "common/iegad_string.h"
#include <functional>


using namespace boost::asio;
using namespace iegad::tools;


iegad::nets::tcp_mt_svr::tcp_mt_svr(
    unsigned short port)
    :
    stop_flag_(false),
    ios_(),
    acptor_(ios_, ip::tcp::endpoint(ip::tcp::v4(), port), true)
{//ctor
    boost::system::error_code err_code;

    acptor_.set_option(ip::tcp::no_delay(true), err_code);
    assert(err_code.value() == 0);
    acptor_.set_option(ip::tcp::socket::reuse_address(true), err_code);
    assert(err_code.value() == 0);
}


iegad::nets::tcp_mt_svr::tcp_mt_svr(
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
    boost::system::error_code err_code = boost::asio::error::bad_descriptor;

    boost::asio::ip::tcp::resolver rlv(acptor_.get_io_service());
    boost::asio::ip::tcp::resolver::query qry(host, svc);

    boost::asio::ip::tcp::resolver::iterator iter = rlv.resolve(qry);
    for (; err_code && iter != end; ++iter) {
	if (acptor_.bind(*iter, err_code) == 0 && err_code.value() == 0) {
	    binded = true;
	    break;
	}
    } // for (; errcode && iter != end; ++iter);
    assert(binded);

    acptor_.set_option(ip::tcp::socket::reuse_address(true), err_code);
    assert(err_code.value() == 0);
    acptor_.set_option(ip::tcp::no_delay(true), err_code);
    assert(err_code.value() == 0);

    acptor_.listen(iegad::nets::tcp_mt_svr::LISTENQ, err_code);
    assert(err_code.value() == 0);
}


void 
iegad::nets::tcp_mt_svr::_thread_proc()
{// thread proc : master thread of server's service.
    io_service ios;
    ip::tcp::socket clnt(ios);
    boost::system::error_code err_code;
    boost::asio::streambuf recvbuff;
    std::string msgstr;

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
	// step 3 : call the service;
	this->_action(clnt, recvbuff);
    } // for (;;);
}


void 
iegad::nets::tcp_mt_svr::run(int n /*= 8*/)
{// run the service
    assert(n > 0);
    iINFO << "@@@ threads num : " << n << " @@@"<<std::endl;
    for (int i = 0; i < n; i++) {
	thread_pool_.push_back(
	    std::thread(std::bind(&tcp_mt_svr::_thread_proc, this)));
    }
}


void 
iegad::nets::tcp_mt_svr::stop()
{// stop the service
    mtx_lock_t locker(stop_mtx_);

    if (stop_flag_) {
	return;
    }

    stop_flag_ = true;
    locker.unlock();
    acptor_.close();    
    for (int i = 0, n = thread_pool_.size(); i < n; i++) {
	thread_pool_[i].join();
    }
    iINFO << "@@@ service's stopped @@@" << std::endl;
}


bool 
iegad::nets::tcp_mt_svr::_is_stop()
{// test the stop_flag_
    mtx_lock_t locker(stop_mtx_);
    return stop_flag_;
}


int 
iegad::nets::tcp_mt_svr::_accept(ip::tcp::socket & clnt, boost::system::error_code & err_code)
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
    if (setsockopt(clnt.native(), SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout_, sizeof(timeout_)) != 0) {
        return -1;
    }
    // 关闭 NAGLE 算法
    clnt.set_option(ip::tcp::no_delay(true));
    
    return 0;
}


iegad::nets::tcp_mt_svr::~tcp_mt_svr()
{
    this->stop();
}


const std::string 
iegad::nets::tcp_mt_svr::host_endpoint()
{
    return acptor_.local_endpoint().address().to_string() 
	+ ":" + iegad::string::to_str(acptor_.local_endpoint().port());
}
