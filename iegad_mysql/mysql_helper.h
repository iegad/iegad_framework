#ifndef __MYSQL_HELPER__
#define __MYSQL_HELPER__


#ifdef WIN32

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "iegad_mysql.lib")

#include <windows.h>

#endif // WIN32


#include "mysql.h"


#include <string>
#include <queue>
#include <mutex>
#include "db/iegad_dbtab.h"


namespace iegad {
namespace mysql {


    class mysql_helper {
    // mysql 连接对象
    public:
	typedef std::unique_lock<std::mutex> lock_t;

	explicit mysql_helper();

	~mysql_helper();


	int open(const std::string & host, unsigned int port,
	    const std::string & usr, const std::string & pwd,
	    const std::string & db, const std::string & charset = "gbk");
	int exec(const std::string & sqlstr);
	int exec_trans(std::queue<std::string> & sqlque);
	int query(const std::string & sqlstr, iegad::db::dbtab_ptr & tab);


    private:
	MYSQL * conn_;
	std::mutex mtx_;

	void _close();

	mysql_helper(const mysql_helper &);
	mysql_helper & operator=(const mysql_helper &);
    }; // class mysql_helper;



} // namespace mysql;
} // namespace iegad;


#endif // __MYSQL_HELPER__