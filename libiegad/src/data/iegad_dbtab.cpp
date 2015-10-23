#include "iegad_dbtab.h"


iegad::data::db_row &
iegad::data::db_tab::operator[](int rownum)
{
    if (static_cast<size_t>(rownum) >= tab_.size()) {
	rownum = tab_.size() - 1;
    }
    return *tab_[rownum];
}


void 
iegad::data::db_tab::add_row(dbrow_ptr & row)
{
    tab_.push_back(row);
}


void 
iegad::data::db_tab::clear()
{
    tab_.clear();
}


int 
iegad::data::db_tab::row_count() const
{
    return tab_.size();
}

iegad::data::db_tab::~db_tab()
{
    this->clear();
}
