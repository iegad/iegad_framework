#include "mysql_conn.h"
#include "iegad_log.h"
#include <assert.h>

using namespace iegad::common;


iegad::mysql::mysql_conn::mysql_conn(const std::string & host, unsigned int port, 
    const std::string & usr, const std::string & pwd,
    const std::string & db, const std::string & charset /*= "gbk"*/)
    : conn_(nullptr), fail_flag_(false)
{
    if (conn_ = mysql_init(nullptr),
	conn_ == nullptr) {
	iSYSERR << "### mysql_init failed ###" << std::endl;
	assert(conn_ != nullptr);
    }
    
    if (mysql_real_connect(conn_, host.c_str(),
	usr.c_str(), pwd.c_str(),
	db.c_str(), port, nullptr, 0) == nullptr) {
	fail_flag_ = true;
    }

    if (mysql_set_character_set(conn_, charset.c_str()) != 0) {
	fail_flag_ = true;
    }
}


iegad::mysql::mysql_conn::~mysql_conn()
{
    mysql_close(conn_);
}


int 
iegad::mysql::mysql_conn::exec(const std::string & sqlstr)
{
    lock_t locker(mtx_);
    fail_flag_ = false;
    if (conn_ == nullptr) {
	fail_flag_ = true;
	return -1;
    }

    if (mysql_query(conn_, sqlstr.c_str()) != 0) {
	fail_flag_ = true;
	return -1;
    }
    return mysql_affected_rows(conn_);
}


int 
iegad::mysql::mysql_conn::exec_trans(std::queue<std::string> & sqlque)
{
    lock_t locker(mtx_);
    fail_flag_ = false;
    std::string sqlstr;
    int n = sqlque.size();   
    if (n == 0) {
	return 0;
    }
    mysql_autocommit(conn_, false);
    for (int i = 0; i < n; i++) {
	sqlstr = sqlque.front();
	sqlque.pop();
	if (mysql_query(conn_, sqlstr.c_str()) != 0) {
	    mysql_rollback(conn_);
	    fail_flag_ = true;
	    return -1;
	}
    }
    mysql_commit(conn_);
    mysql_autocommit(conn_, true);
    return n;
}

MYSQL * 
iegad::mysql::mysql_conn::get()
{
    lock_t locker(mtx_);
    return conn_;
}


const std::string 
iegad::mysql::mysql_conn::err_msg()
{    
    lock_t locker(mtx_);
    return mysql_error(conn_);
}


int 
iegad::mysql::mysql_conn::err_code()
{
    lock_t locker(mtx_);
    return mysql_errno(conn_);
}


bool 
iegad::mysql::mysql_conn::failed()
{
    lock_t locker(mtx_);
    return fail_flag_;
}


MYSQL_RES *
iegad::mysql::mysql_conn::query(const std::string & sqlstr)
{
    lock_t locker(mtx_);
    fail_flag_ = false;
    if (mysql_query(conn_, sqlstr.c_str()) != 0) {
	fail_flag_ = true;
	return nullptr;
    }
    MYSQL_RES * res = mysql_use_result(conn_);
    if (res == nullptr) {
	return nullptr;
    }
    return res;
}
