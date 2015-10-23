#ifndef __IEGAD_DBROW__
#define __IEGAD_DBROW__



// ============ 说明 ============
//
// @创建日期 : 2015-10-03
// @创建人 : iegad
//
// ============================
// @用途 :  数据库访问操作时, 用来存储 "数据行" 的类型, 模访C#中的DataRow
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include <unordered_map>
#include <string>
#include <memory>


namespace iegad {
namespace data {


    class db_row {
    // 数据行
    public:
	// ============================
	// @用途 : 视 散列容器<int, std::string> 为 数据行
	//		 int 为列索引, std::string 为列值, 类型声明
	// ============================
	typedef std::unordered_map<int, std::string> cols_t;


	// ============================
	// @用途 : 构造函数
	// ============================
	explicit db_row() {}


	// ============================
	// @用途 : 构造函数
	// @PS : 在对象释放时, 会清除所有的列.
	// ============================
	~db_row() {
	    this->clear(); 
	}


	// ============================
	// @用途 : [] 运算符重载, 用来访问列值
	// @col_num : 列的索引
	// @返回值 : 该列所包含的值
	// ============================
	const std::string operator[](int col_num);


	// ============================
	// @用途 : 添加一列
	// @col_num : 需要添加的列的索引
	// @col_val : 需要添加的列的值
	// @返回值 : void
	// @PS : 如果 col_num 已存在, 添加将会失败.
	// ============================
	void add_col(int col_num, const std::string col_val);


	// ============================
	// @用途 : 清除所有列的信息
	// @返回值 : void
	// ============================
	void clear();


	// ============================
	// @用途 : 返回列的总数
	// @返回值 : 行中列的总数
	// ============================
	int col_count() const;

    private:
	// 列的集合
	cols_t row_;
	
	// 禁用
	db_row(const db_row &);
	db_row & operator=(const db_row &);
    }; // class db_row;


    // db_row 的智能指针类型
    typedef std::shared_ptr<db_row> dbrow_ptr;


} // namespace data;
} // namespace iegad;


#endif // __IEGAD_DBROW__