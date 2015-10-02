#ifndef __MYSQL_RZT__
#define __MYSQL_RZT__


#ifdef WIN32

#pragma comment(lib, "libmysql.lib")
#include <windows.h>

#endif // WIN32


#include "mysql.h"


namespace iegad {
namespace mysql {


    class mysql_conn;


    class mysql_rzt {
    public:
	mysql_rzt(mysql_conn & conn, MYSQL_RES * res);
	~mysql_rzt();

	MYSQL_RES * get();
	MYSQL_ROW read_row();
	void clear();
    private:
	MYSQL_RES * res_;
	mysql_conn & conn_;
    };


} // namespace mysql;
} // namespace iegad;



#endif // __MYSQL_RZT__