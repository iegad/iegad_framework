#include "iegad_dbrow.h"


const std::string
iegad::db::db_row::operator[](int col_num)
{
    iegad::db::db_row::cols_t::iterator itor = row_.find(col_num);
    if (itor == row_.end()) {
	return "";
    }
    return row_[col_num];
}


void
iegad::db::db_row::add_col(int col_num, const std::string col_val)
{
    iegad::db::db_row::cols_t::iterator itor = row_.find(col_num);
    if (itor != row_.end()) {
	return;
    }
    row_[col_num] = col_val;
}


void 
iegad::db::db_row::clear()
{
    row_.clear();
}


int
iegad::db::db_row::col_count() const
{
    return row_.size();
}
