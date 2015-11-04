#include "mysql_helper.h"
#include "common/iegad_log.h"
#include <assert.h>


using namespace iegad::tools;
using namespace iegad::data;


iegad::mysql::mysql_helper::mysql_helper()
    : open_flag_(false)
{
    assert(mysql_init(&conn_) != nullptr);
}


iegad::mysql::mysql_helper::~mysql_helper()
{
    this->close();
}


int 
iegad::mysql::mysql_helper::exec(const std::string & sqlstr)
{
    lock_t locker(mtx_);

    if (!open_flag_) {
	return -1;
    }

    if (mysql_query(&conn_, sqlstr.c_str()) != 0) {
	return this->_error();
    }

    return static_cast<int>(mysql_affected_rows(&conn_));
}


int 
iegad::mysql::mysql_helper::exec_trans(std::queue<std::string> & sqlque)
{
    lock_t locker(mtx_);

    if (!open_flag_) {
	return -1;
    }

    std::string sqlstr;
    int n = sqlque.size();   

    // disable the auto commit;
    mysql_autocommit(&conn_, false);
    for (int i = 0; i < n; i++) {
	sqlstr = sqlque.front();
	sqlque.pop();
	if (mysql_query(&conn_, sqlstr.c_str()) != 0) {
	    mysql_rollback(&conn_);	    
	    return this->_error();
	}
    }
    mysql_commit(&conn_);
    mysql_autocommit(&conn_, true);
    return n;
}


int 
iegad::mysql::mysql_helper::open(const std::string & host, unsigned int port,
						    const std::string & usr, const std::string & pwd,
						    const std::string & db, const std::string & charset/* = "gbk"*/)
{
    lock_t locker(mtx_);

    if (open_flag_) {
	return 0;
    }

    if (mysql_real_connect(&conn_, host.c_str(),
	usr.c_str(), pwd.c_str(),
	db.c_str(), port, nullptr, 0) == nullptr) {
	this->close();
	open_flag_ = false;
	return this->_error();
    }

    if (mysql_set_character_set(&conn_, charset.c_str()) != 0) {
	this->close();
	errstr_ = mysql_error(&conn_);
	return this->_error();
    }
    open_flag_ = true;
    return 0;
}


void 
iegad::mysql::mysql_helper::close()
{
    lock_t locker(mtx_);

    if (open_flag_) {
	mysql_close(&conn_);
	open_flag_ = false;
    }
}


int
iegad::mysql::mysql_helper::query(const std::string & sqlstr, iegad::data::db_tab & tab)
{
    lock_t locker(mtx_);

    if (!open_flag_) {
	return -1;
    }

    if (mysql_real_query(&conn_, sqlstr.c_str(), sqlstr.size()) != 0) {
	return this->_error();
    }

    MYSQL_RES * res; 
    MYSQL_ROW row;
    unsigned int n;
    do {
	if (res = mysql_use_result(&conn_), res != nullptr) {
	    n = mysql_field_count(&conn_);
	    while (row = mysql_fetch_row(res), row != nullptr) {
		if (row == nullptr) {
		    mysql_free_result(res);
		    if (mysql_errno(&conn_) == 0) {
			return 0;
		    }
		    else {
			return this->_error();
		    }
		}

		db_row * dbrow = new db_row;
		for (unsigned int i = 0; i < n; i++) {
		    dbrow->add_col(i, row[i] != nullptr ? row[i] : "");
		}
		tab.add_row(dbrow_ptr(dbrow));
	    } // while (row = mysql_fetch_row(res), row != nullptr);
	} // if (res = mysql_use_result(conn_), res != nullptr);
    } while (n = mysql_next_result(&conn_), n == 0);

    mysql_free_result(res);

    return 0;
}


int
iegad::mysql::mysql_helper::call_proc(const std::string & sqlstr)
{
    lock_t locker(mtx_);

    if (!open_flag_) {
	return -1;
    }

    if (mysql_real_query(&conn_, sqlstr.c_str(), sqlstr.size()) != 0) {
	return this->_error();
    }

    MYSQL_RES * res = mysql_store_result(&conn_);
    if (res == nullptr) {
	return 0;
    }

    int n = mysql_field_count(&conn_);
    MYSQL_ROW row;

    do {
	row = mysql_fetch_row(res);
    } while (mysql_next_result(&conn_) == 0);
    mysql_free_result(res);

    return 0;
}


int
iegad::mysql::mysql_helper::call_proc(const std::string & sqlstr, 
    std::vector<std::string> & out_param)
{
    lock_t locker(mtx_);

    if (!open_flag_) {
	return -1;
    }

    MYSQL_ROW row;
    MYSQL_RES * res;

    if (mysql_real_query(&conn_, sqlstr.c_str(), sqlstr.size()) != 0) {
	iWARN << mysql_errno(&conn_) << "\t\t" << mysql_error(&conn_) << std::endl;
	return this->_error();
    }

    res = mysql_store_result(&conn_);
    if (res == nullptr) {
	return 0;
    }

    int n = mysql_field_count(&conn_);

    do {
	if (row = mysql_fetch_row(res), row == nullptr) {
	    continue;
	}

	for (int i = 0; i < n; i++) {
	    out_param.push_back(row[i]);
	}

    } while (mysql_next_result(&conn_) == 0);

    mysql_free_result(res);

    return 0;
}


int 
iegad::mysql::mysql_helper::_error()
{
    lock_t locker(mtx_);
    errstr_ = mysql_error(&conn_);
    return -(static_cast<int>(mysql_errno(&conn_)));
}


const std::string & 
iegad::mysql::mysql_helper::errmsg() const
{
    lock_t locker(mtx_);
    return errstr_;
}






