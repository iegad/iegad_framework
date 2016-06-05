#include <iostream>
#include "data_obj_gen.hpp"


using namespace oa_system::dbo;


int 
main(int argc, char * argv[])
{
    soci::session sql;

    try {
	sql.open("mysql://db=test user=root password=1111");

	soci::rowset<std::string> rows(sql.prepare << "SELECT NAME FROM PERSON_T");
	
	CHECK(rows.begin() == rows.end());
	for (auto itor = rows.begin(); itor != rows.end(); itor++) {
	}
    }
    catch (std::exception & ex) {
	std::cout << ex.what() << std::endl;
    }
    std::cin.get();
    exit(0);
}
