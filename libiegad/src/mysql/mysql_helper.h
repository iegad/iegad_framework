#ifndef __MYSQL_HELPER__
#define __MYSQL_HELPER__


// ============ 说明 ============
//
// @创建日期 : 2015-10-03
// @创建人 : iegad
//
// ============================
// @用途 :  MySQL 数据库访问操作 对象
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================
// --2015-11-04	--iegad		    1, 取消日志.
//							    2, 将内置MYSQL指针, 换成对象.
//							    3, 添加 error 信息


#ifdef WIN32
#include <windows.h>
#endif // WIN32


#include <mysql/mysql.h>


#include <string>
#include <queue>
#include <mutex>
#include "data/iegad_dbtab.h"


namespace iegad {
namespace mysql {


    class mysql_helper {
    // mysql 操作对象
    public:
	// ============================
	// @用途 : 工具锁 类型声明
	// ============================
	typedef std::unique_lock<std::mutex> lock_t;


	// ============================
	// @用途 : 构造函数
	// @PS : 会初始化 mysql 内置指针, 
	//	    如果失败, 所在应用程序会异常退出
	// ============================
	explicit mysql_helper();


	// ============================
	// @用途 : 构造函数
	// @PS : 在对象释放时, 会关闭数据库连接.
	// ============================
	~mysql_helper();


	// ============================
	// @用途 : 打开数据库连接
	// @host : mysql服务器 IP 或 机器名
	// @port : 端口
	// @usr : 用户名
	// @pwd : 密码
	// @db : 数据库名称
	// @charset : 字符集
	// @返回值 : 成功连接返回0, 否则返回 errno;
	// ============================
	int open(const std::string & host, unsigned int port,
	    const std::string & usr, const std::string & pwd,
	    const std::string & db, const std::string & charset = "gbk");


	// ============================
	// @用途 : 执行DML操作
	// @sqlstr : DML语句
	// @返回值 : 成功, 返回受影响的行数, 否则返回 errno;
	// ============================
	int exec(const std::string & sqlstr);


	// ============================
	// @用途 : 执行DML事务操作
	// @sqlque : 需要执行的秘有DML语句
	// @返回值 : 成功, 返回受影响的行数, 否则返回 errno;
	// ============================
	int exec_trans(std::queue<std::string> & sqlque);


	// ============================
	// @用途 : 执行查询操作, 并填充 数据表tab
	// @sqlstr : DQL语句
	// @tab : 用来保存数据的 数据表智能指针
	// @返回值 : 成功返回0, 否则返回 errno;
	// ============================
	int query(const std::string & sqlstr, iegad::data::db_tab & tab);


	// ============================
	// @用途 : 执行存储过程
	// @sqlstr : 存储过程调用语句
	// @返回值 : 成功返回0, 否则返回 errno;
	// @PS : 这里返回成功只是说明 MySQL API 调用成功, 
	//	    并不代表存储过程本身执行成功;
	// ============================
	int call_proc(const std::string & sqlstr);


	// ============================
	// @用途 : 执行存储过程
	// @sqlstr : 存储过程调用语句
	// @out_param : 用来接收输出参数
	// @返回值 : 成功返回0, 否则返回 errno;
	// ============================
	int call_proc(const std::string & sqlstr, std::vector<std::string> & out_param);


	// ============================
	// @用途 : 关闭 mysql 连接
	// @返回值 : void
	// ============================
	void close();

	// ============================
	// @用途 : 返回最后一次出错的 errstr
	// @返回值 : errstr
	// ============================
	const std::string & errmsg() const;


    private:
	// ============================
	// @用途 : 记录 errstr 并返回错误码.
	// @返回值 : 错误码.
	// @PS : 为了区别 错误码 和 受影响的行数, 错误码统一为负数.
	// ============================
	int _error();


	// 状态标志
	bool open_flag_;
	// 操作锁
	mutable std::mutex mtx_;
	// 最后错误信息
	std::string errstr_;
	// MYSQL 操作句柄
	MYSQL conn_;


	// 禁用
	mysql_helper(const mysql_helper &);
	mysql_helper & operator=(const mysql_helper &);
    }; // class mysql_helper;



} // namespace mysql;
} // namespace iegad;


#endif // __MYSQL_HELPER__