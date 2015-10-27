#ifndef __SQLITE_HELPER__
#define __SQLITE_HELPER__



#include "sqlite/sqlite3.h"
#include <string>
#include <vector>
#include <mutex>


#include "data/iegad_dbtab.h"


namespace iegad {
namespace sqlite {


    class sqlite_helper {
    public:
	typedef std::unique_lock<std::mutex> lock_t;

	explicit sqlite_helper()
	    : opened_(false), conn_(nullptr) {}

	~sqlite_helper();

	int open(const std::string &filename);

	int exec(const std::string & sqlstr);

	int exec_trans(const std::vector<std::string> & sqlvct);

	int query(const std::string & sqlstr, iegad::data::db_tab & tab);

	int call_proc(const std::string & sqlstr);

	int call_proc(const std::string & sqlstr, std::vector<std::string> & out_parm);

	void close();

	const std::string last_errmsg();
	
    private:

	//static int _query_callback(void * notused, int argc, char ** argv, char ** col_name);

	bool opened_;
	sqlite3 * conn_;
	std::mutex mtx_;
	std::string errmsg_;

    };


} // namespace sqlite;
} // namespace iegad;



#endif // __SQLITE_HELPER__