#include "up_usrinfo_svc.h"
#include "msg/iegad_app_svr_msg.pb.h"
#include "common/iegad_log.h"
#include "iegad_mysql.h"
#include "string/iegad_string.h"


using namespace iegad::common;
using namespace iegad::mysql;


int 
up_usrinfo_svc::action(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr)
{
    up_usrinfo_msg msg;
    mysql_helper mh;
    if (!msg.ParseFromString(msg_bdstr)) {
	iWARN << "### msg.ParseFromString(msg_bdstr) ###" << std::endl;
	return -1;
    }

    if (mh.open(MH_HOST, MH_PORT, MH_USR, MH_PWD, MH_DB) != 0) {
	return -1;
    }

    std::string sqlstr = "UPDATE USR_LOG_INFO_T SET U_NICK_NAME = '" + msg.nick_name() + "'," +
	"U_NAME = '" + msg.name() + "'," +
	"U_SEX = '" + msg.sex() + "'," +
	"U_BIRTH = " + (msg.birth() == "" ? "NULL" : "'" + msg.birth() + "'") + "," +
	"U_HOMETOWN = '" + msg.hometown() + "'," +
	"U_LOCATION = '" + msg.location() + "'," +
	"U_BLOOD = '" + msg.blood() + "'," +
	"U_TEL = '" + msg.tel() + "'," +
	"U_JOB = " + iegad::string::to_str(msg.job()) + "," +
	"U_SCHOOL = '" + msg.school() + "' WHERE U_LOG_ID = '" + msg.log_id() + "'";

    if (mh.exec(sqlstr) != 1) {
	return -1;
    }
    this->_response(clnt, "0", 1);
    return 0;
}


up_usrinfo_svc::up_usrinfo_svc()
    : iegad::net::svc_basic(SVC_UP_USRINFO)
{}
