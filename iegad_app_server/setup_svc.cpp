#include "setup_svc.h"
#include "msg/iegad_app_svr_msg.pb.h"
#include "common/iegad_log.h"
#include "iegad_mysql.h"
#include "string/iegad_string.h"


using namespace iegad::common;
using namespace iegad::mysql;


int 
setup_svc::action(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr)
{
    setup_msg msg;
    mysql_helper mh;
    if (!msg.ParseFromString(msg_bdstr)) {
	iWARN << "### msg_.ParseFromString(msg_bdstr) failed ###" << std::endl;
	return -1;
    }

    if (mh.open(MH_HOST, MH_PORT, MH_USR, MH_PWD, MH_DB) != 0) {
	return -1;
    }

    std::string sqlstr = "CALL SET_UP_P ('" + msg.log_id() + "', '" + msg.pwd() + "', '" + msg.email() + "')";
    std::vector<std::string> outpa;
    if (mh.call_proc(sqlstr, outpa) != 0) {
	return -1;
    }
    int rzt = iegad::string::to_int32(outpa[0]);
    if (rzt == 0) {
	rzt = this->_response(clnt, "0", 1);
    }
    return rzt;
}

setup_svc::setup_svc()
    : iegad::net::svc_basic(SVC_SETUP)
{}
