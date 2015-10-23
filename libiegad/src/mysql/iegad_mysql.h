#ifndef __IEGAD_MYSQL__
#define __IEGAD_MYSQL__


#ifdef WIN32
// windows 平台下, 静态库引用
#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "iegad_mysql.lib")
#endif // WIN32



#include "mysql_helper.h"


#endif // __IEGAD_MYSQL__