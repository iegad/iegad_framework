#ifndef __MYSQL_CONN__
#define __MYSQL_CONN__



#include "mysql_rzt.h"

#include <string>
#include <queue>
#include <mutex>


namespace iegad {
namespace mysql {


    class mysql_conn {
    // mysql 连接对象
    public:
	friend class mysql_rzt;
	typedef std::unique_lock<std::mutex> lock_t;

	explicit mysql_conn(const std::string & host, unsigned int port, 
	    const std::string & usr, const std::string & pwd,
	    const std::string & db, const std::string & charset = "gbk");

	~mysql_conn();


	int exec(const std::string & sqlstr);
	int exec_trans(std::queue<std::string> & sqlque);
	MYSQL_RES * query(const std::string & sqlstr);

	MYSQL * get();
	const std::string err_msg();
	int err_code();
	bool failed();

    private:
	bool fail_flag_;
	MYSQL * conn_;
	std::mutex mtx_;

	mysql_conn(const mysql_conn &);
	mysql_conn & operator=(const mysql_conn &);
    }; // class mysql_conn;



} // namespace mysql;
} // namespace iegad;


#endif // __MYSQL_CONN__