#include "iegad_dbrow.h"


const std::string
iegad::data::db_row::operator[](int col_num)
{
    iegad::data::db_row::cols_t::iterator itor = row_.find(col_num);
    if (itor == row_.end()) {
	return "";
    }
    return row_[col_num];
}


void
iegad::data::db_row::add_col(int col_num, const std::string col_val)
{
    iegad::data::db_row::cols_t::iterator itor = row_.find(col_num);
    if (itor != row_.end()) {
	return;
    }
    row_[col_num] = col_val;
}


void 
iegad::data::db_row::clear()
{
    row_.clear();
}


int
iegad::data::db_row::col_count() const
{
    return row_.size();
}
