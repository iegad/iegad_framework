#include "sqlite_helper.h"


iegad::sqlite::sqlite_helper::~sqlite_helper()
{
    if (opened_) {
	this->close();
    }
}


int 
iegad::sqlite::sqlite_helper::exec(const std::string & sqlstr)
{
    char * err;
    if (sqlite3_exec(conn_, sqlstr.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
	errmsg_ = err;
    }
    return 0;
}


int 
iegad::sqlite::sqlite_helper::exec_trans(const std::vector<std::string> & sqlvct)
{
    char * err = nullptr;
    if (sqlite3_exec(conn_, "BEGIN", nullptr, nullptr, &err) != SQLITE_OK) {
	errmsg_ = err;
    }

    for (int i = 0, n = sqlvct.size(); i < n; i++) {
	if (sqlite3_exec(conn_, sqlvct[i].c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
	    errmsg_ = err;
	    break;
	}
    }

    if (err == nullptr) {
	if (sqlite3_exec(conn_, "COMMIT", nullptr, nullptr, &err) != SQLITE_OK) {
	    errmsg_ = err;
	    return -1;
	}
	return sqlvct.size();
    }
    else {
	sqlite3_exec(conn_, "ROLLBACK", nullptr, nullptr, &err);
	return -1;
    }
}


void 
iegad::sqlite::sqlite_helper::close()
{
    sqlite3_close(conn_);
    opened_ = false;
}


int 
iegad::sqlite::sqlite_helper::open(const std::string &filename)
{
    if (sqlite3_open(filename.c_str(), &conn_) != SQLITE_OK) {
	opened_ = false;
	return -1;
    }
    opened_ = true;
    return 0;
}


int 
iegad::sqlite::sqlite_helper::query(const std::string & sqlstr, iegad::data::db_tab & tab)
{
    sqlite3_stmt * stmt = nullptr;
    int cn, rn;
    if (sqlite3_prepare(conn_, sqlstr.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
	errmsg_ = sqlite3_errmsg(conn_);
	return -1;
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
	    return -1;
	}
    }
    return 0;
}

const std::string iegad::sqlite::sqlite_helper::last_errmsg()
{
    return errmsg_;
}
