#ifndef __SQLITE_HELPER__
#define __SQLITE_HELPER__


#include "sqlite3.h"
#include <string>
#include <mutex>


namespace iegad {
namespace dbctl {


    class sqlite_helper {
    public:
	sqlite_helper() : handle_(nullptr), open_flag_(false) {}	

	~sqlite_helper();

	int exec_ddl(const std::string & ddlstr);
	int exec_dml(const std::string & sqlstr);
	int open(const std::string & db_filepath);
	void close();
	const std::string get_last_errmsg();
	bool is_open();

    private:
	void _set_errmsg();
	void _set_errmsg(const char * errstr);
	void _open();
	void _close();

	bool open_flag_;
	sqlite3 * handle_;
	std::string errmsg_;
	std::mutex open_flag_mtx_;
	std::mutex err_mtx_;

	sqlite_helper(const sqlite_helper &);
	sqlite_helper & operator=(const sqlite_helper &);
    };


} // namespace dbctl;
} // namespace iegad



#endif // __SQLITE_HELPER__