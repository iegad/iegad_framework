#include "sqlite_helper.h"


int
iegad::dbctl::sqlite_helper::open(const std::string & db_filepath)
{
    if (sqlite3_open(db_filepath.c_str(), &handle_) == SQLITE_OK) {
	this->_open();
	return 0;
    }
    this->_set_errmsg();
    return -1;
}


void
iegad::dbctl::sqlite_helper::_set_errmsg()
{
    std::unique_lock<std::mutex> locker(err_mtx_);
    errmsg_ = sqlite3_errmsg(handle_);
}


void iegad::dbctl::sqlite_helper::_set_errmsg(const char * errstr)
{
    std::unique_lock<std::mutex> locker(err_mtx_);
    errmsg_ = errstr;
}


const std::string 
iegad::dbctl::sqlite_helper::get_last_errmsg()
{
    std::unique_lock<std::mutex> locker(err_mtx_);
    return errmsg_;
}


bool 
iegad::dbctl::sqlite_helper::is_open()
{
    std::unique_lock<std::mutex> locker(open_flag_mtx_);
    return open_flag_;
}


void 
iegad::dbctl::sqlite_helper::_open()
{
    std::unique_lock<std::mutex> locker(open_flag_mtx_);
    open_flag_ = true;
}


void 
iegad::dbctl::sqlite_helper::_close()
{
    std::unique_lock<std::mutex> locker(open_flag_mtx_);
    open_flag_ = false;
}


int 
iegad::dbctl::sqlite_helper::exec_ddl(const std::string & ddlstr)
{
    char * errstr = nullptr;
    if (sqlite3_exec(handle_, ddlstr.c_str(), nullptr, 0, &errstr) != SQLITE_OK) {
	this->_set_errmsg(errstr);
	sqlite3_free(errstr);
	return -1;
    }
    return 0;
}


int 
iegad::dbctl::sqlite_helper::exec_dml(const std::string & sqlstr)
{
    char * errstr = nullptr;
    if (sqlite3_exec(handle_, sqlstr.c_str(), nullptr, 0, &errstr) != SQLITE_OK) {
	this->_set_errmsg(errstr);
	sqlite3_free(errstr);
	return -1;
    }
    return 0;
}


void
iegad::dbctl::sqlite_helper::close()
{
    if (this->is_open()) {
	this->_close();
	sqlite3_close(handle_);
	handle_ = nullptr;
    }
}


iegad::dbctl::sqlite_helper::~sqlite_helper()
{
    this->close();
}
