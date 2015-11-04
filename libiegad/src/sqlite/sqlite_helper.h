#ifndef __SQLITE_HELPER__
#define __SQLITE_HELPER__



// ============ 说明 ============
//
// @创建日期 : 2015-11-04
// @创建人 : iegad
//
// ============================
// @用途 :  SQLite 数据库访问操作 对象
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include "sqlite/sqlite3.h"
#include <string>
#include <vector>
#include <mutex>


#include "data/iegad_dbtab.h"


namespace iegad {
namespace sqlite {


    class sqlite_helper {
    // sqlite 操作对象
    public:
	// ============================
	// @用途 : 工具锁 类型声明
	// ============================
	typedef std::unique_lock<std::mutex> lock_t;


	// ============================
	// @用途 : 构造函数
	// ============================
	explicit sqlite_helper();


	// ============================
	// @用途 : 构造函数
	// @PS : 在对象释放时, 会关闭数据库连接.
	// ============================
	~sqlite_helper();


	// ============================
	// @用途 : 打开数据库连接
	// @filename : sqlite db 文件的路径
	// @返回值 : 成功连接返回0, 否则返回 errno;
	// ============================
	int open(const std::string &filename);


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
	int exec_trans(const std::vector<std::string> & sqlvct);


	// ============================
	// @用途 : 执行查询操作, 并填充 数据表tab
	// @sqlstr : DQL语句
	// @tab : 用来保存数据的 数据表智能指针
	// @返回值 : 成功返回0, 否则返回 errno;
	// ============================
	int query(const std::string & sqlstr, iegad::data::db_tab & tab);


	// ============================
	// @用途 : <无用>
	// @PS : 保留, 只是为了和其实 db_helper 有相同的调用接口
	// ============================
	int call_proc(const std::string & sqlstr) {
	    return -1;
	}


	// ============================
	// @用途 : <无用>
	// @PS : 保留, 只是为了和其实 db_helper 有相同的调用接口
	// ============================
	int call_proc(const std::string & sqlstr, std::vector<std::string> & out_parm) {
	    return -1;
	}


	// ============================
	// @用途 : 关闭 sqlite 连接
	// @返回值 : void
	// ============================
	void close();


	// ============================
	// @用途 : 返回最后一次出错的 errstr
	// @返回值 : errstr
	// ============================
	const std::string & errmsg() const;
	
    private:
	// 状态标志
	bool open_flag_;
	// 操作句柄
	sqlite3 * conn_;
	// 操作锁
	mutable std::mutex mtx_;
	// 最后错误信息
	std::string errmsg_;


	// 禁用
	sqlite_helper(const sqlite_helper &);
	sqlite_helper & operator=(const sqlite_helper &);
    }; // class sqlite_helper;


} // namespace sqlite;
} // namespace iegad;



#endif // __SQLITE_HELPER__