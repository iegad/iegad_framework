#include "mysql_rzt.h"
#include "mysql_conn.h"


iegad::mysql::mysql_rzt::mysql_rzt(mysql_conn & conn, MYSQL_RES * res)
    : res_(res), conn_(conn)
{}


iegad::mysql::mysql_rzt::~mysql_rzt()
{
    mysql_free_result(res_);
}


MYSQL_RES * 
iegad::mysql::mysql_rzt::get()
{
    return res_;
}


MYSQL_ROW
iegad::mysql::mysql_rzt::read_row()
{
    conn_.fail_flag_ = false;
    MYSQL_ROW row;
    if (conn_.get() == nullptr || res_ == nullptr) {
	conn_.fail_flag_ = true;
	return nullptr;
    }
    int n;
    row = mysql_fetch_row(res_);
    if (n = mysql_next_result(conn_.get()), 
	n == -1) {
	return nullptr;
    }
    return row;
}


void 
iegad::mysql::mysql_rzt::clear()
{
    mysql_free_result(res_);
}
