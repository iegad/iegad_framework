#include "mysql_helper.h"
#include "common/iegad_log.h"
#include <assert.h>


using namespace iegad::common;
using namespace iegad::db;


iegad::mysql::mysql_helper::mysql_helper()
    : conn_(nullptr)
{
    if (conn_ = mysql_init(nullptr),
	conn_ == nullptr) {
	iSYSERR << "### mysql_init failed ###" << std::endl;
	assert(conn_ != nullptr);
    }
}


iegad::mysql::mysql_helper::~mysql_helper()
{
    this->_close();
}


int 
iegad::mysql::mysql_helper::exec(const std::string & sqlstr)
{
    lock_t locker(mtx_);
    if (conn_ == nullptr || mysql_query(conn_, sqlstr.c_str()) != 0) {
	return -1;
    }
    return mysql_affected_rows(conn_);
}


int 
iegad::mysql::mysql_helper::exec_trans(std::queue<std::string> & sqlque)
{
    lock_t locker(mtx_);
    std::string sqlstr;
    int n = sqlque.size();   
    if (n == 0 || conn_ == nullptr) {
	return -1;
    }

    mysql_autocommit(conn_, false);
    for (int i = 0; i < n; i++) {
	sqlstr = sqlque.front();
	sqlque.pop();
	if (mysql_query(conn_, sqlstr.c_str()) != 0) {
	    mysql_rollback(conn_);
	    return -1;
	}
    }
    mysql_commit(conn_);
    mysql_autocommit(conn_, true);
    return n;
}


int 
iegad::mysql::mysql_helper::open(const std::string & host, unsigned int port,
						    const std::string & usr, const std::string & pwd,
						    const std::string & db, const std::string & charset/* = "gbk"*/)
{
    if (mysql_real_connect(conn_, host.c_str(),
	usr.c_str(), pwd.c_str(),
	db.c_str(), port, nullptr, 0) == nullptr) {
	iSYSERR << "### mysql_real_connect failed ###" << std::endl;
	this->_close();
	return -1;
    }

    if (mysql_set_character_set(conn_, charset.c_str()) != 0) {
	iSYSERR << "### mysql_set_character_set failed ###" << std::endl;
	this->_close();
	return -1;
    }
    return 0;
}


void 
iegad::mysql::mysql_helper::_close()
{
    lock_t locker(mtx_);
    mysql_close(conn_);
}


int
iegad::mysql::mysql_helper::query(const std::string & sqlstr, iegad::db::dbtab_ptr & tab)
{
    lock_t locker(mtx_);
    if (conn_ == nullptr) {
	return -1;
    }

    if (mysql_real_query(conn_, sqlstr.c_str(), sqlstr.size()) != 0) {
	iWARN << mysql_errno(conn_) << "\t\t" << mysql_error(conn_) << std::endl;
	return -1;
    }

    MYSQL_RES * res; 
    MYSQL_ROW row;
    unsigned int n;
    do {
	if (res = mysql_use_result(conn_), res != nullptr) {
	    n = mysql_field_count(conn_);
	    while (row = mysql_fetch_row(res), row != nullptr) {
		if (row == nullptr) {
		    mysql_free_result(res);
		    if (mysql_errno(conn_) == 0) {
			return 0;
		    }
		    else {
			iWARN << mysql_errno(conn_) << "\t\t" << mysql_error(conn_) << std::endl;
			return -1;
		    }
		}

		db_row * dbrow = new db_row;
		for (int i = 0; i < n; i++) {
		    dbrow->add_col(i, row[i] != nullptr ? row[i] : "");
		}
		tab->add_row(dbrow_ptr(dbrow));
	    } // while (row = mysql_fetch_row(res), row != nullptr);
	} // if (res = mysql_use_result(conn_), res != nullptr);
    } while (n = mysql_next_result(conn_), n == 0);

    mysql_free_result(res);

    return 0;
}


int
iegad::mysql::mysql_helper::call_proc(const std::string & sqlstr)
{
    if (mysql_real_query(conn_, sqlstr.c_str(), sqlstr.size()) != 0) {
	iWARN << mysql_errno(conn_) << "\t\t" << mysql_error(conn_) << std::endl;
	return -1;
    }
    MYSQL_RES * res = mysql_store_result(conn_);
    if (res == nullptr) {
	return 0;
    }
    int n = mysql_field_count(conn_);
    MYSQL_ROW row;
    do {
	row = mysql_fetch_row(res);
    } while (mysql_next_result(conn_) == 0);
    mysql_free_result(res);
    return 0;
}


int
iegad::mysql::mysql_helper::call_proc(const std::string & sqlstr, 
    std::vector<std::string> & out_param)
{
    MYSQL_ROW row;
    MYSQL_RES * res;

    if (mysql_real_query(conn_, sqlstr.c_str(), sqlstr.size()) != 0) {
	iWARN << mysql_errno(conn_) << "\t\t" << mysql_error(conn_) << std::endl;
	return -1;
    }

    res = mysql_store_result(conn_);
    if (res == nullptr) {
	return 0;
    }

    int n = mysql_field_count(conn_);

    do {
	if (row = mysql_fetch_row(res), row == nullptr) {
	    continue;
	}

	for (int i = 0; i < n; i++) {
	    out_param.push_back(row[i]);
	}

    } while (mysql_next_result(conn_) == 0);
    mysql_free_result(res);
    return 0;
}






