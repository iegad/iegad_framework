#include "sqlite_helper.h"


iegad::sqlite::sqlite_helper::sqlite_helper()
    : open_flag_(false), conn_(nullptr)
{}


iegad::sqlite::sqlite_helper::~sqlite_helper()
{
    lock_t lock(mtx_);
    if (open_flag_) {
	this->close();
    }
}


int 
iegad::sqlite::sqlite_helper::exec(const std::string & sqlstr)
{
    lock_t lock(mtx_);

    if (!open_flag_) {
	return -1;
    }

    char * err;
    if (sqlite3_exec(conn_, sqlstr.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
	errmsg_ = err;
	return -sqlite3_errcode(conn_);
    }
    return 0;
}


int 
iegad::sqlite::sqlite_helper::exec_trans(const std::vector<std::string> & sqlvct)
{
    lock_t lock(mtx_);

    if (!open_flag_) {
	return -1;
    }

    char * err = nullptr;
    // step 1 : transaction begin;
    if (sqlite3_exec(conn_, "BEGIN", nullptr, nullptr, &err) != SQLITE_OK) {
	errmsg_ = err;
	return -sqlite3_errcode(conn_);
    }
    // step 2 : execute the sqlcmds;
    for (int i = 0, n = sqlvct.size(); i < n; i++) {
	if (sqlite3_exec(conn_, sqlvct[i].c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
	    errmsg_ = err;
	    break;
	}
    }
    // step 3 : test err;
    if (err == nullptr) {
	// no err commit;
	if (sqlite3_exec(conn_, "COMMIT", nullptr, nullptr, &err) != SQLITE_OK) {
	    errmsg_ = err;
	    return -sqlite3_errcode(conn_);
	}
	return sqlvct.size();
    }
    else {
	// err rollback;
	sqlite3_exec(conn_, "ROLLBACK", nullptr, nullptr, &err);
	return -sqlite3_errcode(conn_);
    }
}


void 
iegad::sqlite::sqlite_helper::close()
{
    lock_t lock(mtx_);
    if (open_flag_) {
	sqlite3_close(conn_);
	open_flag_ = false;
    }
}


int 
iegad::sqlite::sqlite_helper::open(const std::string &filename)
{
    lock_t lock(mtx_);

    if (open_flag_) {
	return 0;
    }

    if (sqlite3_open(filename.c_str(), &conn_) != SQLITE_OK) {
	open_flag_ = false;
	return -sqlite3_errcode(conn_);
    }
    open_flag_ = true;
    return 0;
}


int 
iegad::sqlite::sqlite_helper::query(const std::string & sqlstr, iegad::data::db_tab & tab)
{
    lock_t lock(mtx_);

    if (!open_flag_) {
	return -1;
    }

    sqlite3_stmt * stmt = nullptr;
    int cn, rn;

    if (sqlite3_prepare(conn_, sqlstr.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
	errmsg_ = sqlite3_errmsg(conn_);
	return -sqlite3_errcode(conn_);
    }

    cn = sqlite3_column_count(stmt);

    while (true) {
	rn = sqlite3_step(stmt);
	if (rn == SQLITE_DONE) {
	    break;
	} 
	else if (rn == SQLITE_ROW) {
        iegad::data::dbrow_ptr row(new iegad::data::db_row);
	    for (int i = 0; i < cn; i++) {
		row->add_col(i, (const char *)sqlite3_column_text(stmt, i));
	    }
        tab.add_row(row);
	}
	else {
	    sqlite3_finalize(stmt);
	    errmsg_ = sqlite3_errmsg(conn_);
	    return -sqlite3_errcode(conn_);
	}
    }
    return 0;
}

const std::string &
iegad::sqlite::sqlite_helper::errmsg() const
{
    lock_t lock(mtx_);
    return errmsg_;
}


