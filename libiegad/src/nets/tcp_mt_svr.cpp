#include "tcp_mt_svr.h"
#include "msg/basic_msg.pb.h"
#include "common/iegad_log.h"
#include "msg/iegad_msg.h"
#include "common/iegad_string.h"


using namespace boost::asio;
using namespace iegad::tools;


iegad::nets::tcp_mt_svr::tcp_mt_svr(
    unsigned short port)
    :
    stop_flag_(false),
    ios_(),
    acptor_(ios_, ip::tcp::endpoint(ip::tcp::v4(), port), true)
{//ctor
    // set TCP_NAGLE off
    acptor_.set_option(ip::tcp::no_delay(true));
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

	// step 3 : get the msg_dbstr;
	if (msgstr = this->_get_msgstr(clnt, recvbuff, err_code), 
	    msgstr == ERR_STRING) {
	    continue;
	}
	// step 4 : call the service;
	this->_action(clnt, msgstr);
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


const std::string
iegad::nets::tcp_mt_svr::_get_msgstr(ip::tcp::socket & clnt,
boost::asio::streambuf & recvbuff,
boost::system::error_code & err_code)
{
    std::string res(ERR_STRING);
    res = iegad::msg::recv_str(clnt, recvbuff, err_code, MSG_KEY);
    if (err_code == boost::asio::error::timed_out) {
	iWARN << clnt.remote_endpoint().address().to_string() << " ### TIME OUT ###" << std::endl;
    }
    return res;
}


iegad::nets::tcp_mt_svr::~tcp_mt_svr()
{
    this->stop();
}


const std::string 
iegad::nets::tcp_mt_svr::host_endpoint()
{
    return acptor_.local_endpoint().address().to_string() + ":" + iegad::string::to_str(acptor_.local_endpoint().port());
}
