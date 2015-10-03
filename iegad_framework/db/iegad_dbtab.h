#ifndef __IEGAD_DBTAB__
#define __IEGAD_DBTAB__


#include "iegad_dbrow.h"
#include <vector>


namespace iegad {
namespace db {


    class db_tab {
    public:	
	typedef std::vector<dbrow_ptr> rows_t;

	explicit db_tab() {}
	~db_tab() {}

	dbrow_ptr operator[](int rownum);
	void add_row(dbrow_ptr & row);
	void clear();
	int row_count() const;

    private:
	rows_t tab_;

	db_tab(db_tab &);
	db_tab & operator=(db_tab &);
    }; // class db_tab;


    typedef std::shared_ptr<db_tab> dbtab_ptr;


} // namespace db;
} // namespace iegad;


#endif // __IEGAD_DBTAB__