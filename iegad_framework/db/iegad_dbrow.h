#ifndef __IEGAD_DBROW__
#define __IEGAD_DBROW__



#include <unordered_map>
#include <string>
#include <memory>


namespace iegad {
namespace db {


    class db_row {
    public:
	typedef std::unordered_map<int, std::string> cols_t;

	explicit db_row() {}
	~db_row() {}

	const std::string operator[](int col_num);
	void add_col(int col_num, const std::string col_val);
	void clear();
	int col_count() const;

    private:
	cols_t row_;
	
	db_row(const db_row &);
	db_row & operator=(const db_row &);
    }; // class db_row;

    typedef std::shared_ptr<db_row> dbrow_ptr;


} // namespace db;
} // namespace iegad;


#endif // __IEGAD_DBROW__