#include "iegad_dbtab.h"


iegad::db::db_row &
iegad::db::db_tab::operator[](int rownum)
{
    if (rownum >= tab_.size()) {
	rownum = tab_.size() - 1;
    }
    return *tab_[rownum];
}


void 
iegad::db::db_tab::add_row(dbrow_ptr & row)
{
    tab_.push_back(row);
}


void 
iegad::db::db_tab::clear()
{
    tab_.clear();
}


int 
iegad::db::db_tab::row_count() const
{
    return tab_.size();
}
