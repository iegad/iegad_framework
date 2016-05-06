#include <iostream>
#include "data_obj_gen.hpp"


using namespace oa_system::dbo;


int 
main(int argc, char * argv[])
{
    soci::session sql;

    try {
	sql.open("mysql://db=test user=root password=1111");
	Person p;
	p.__set_Age(18);
	p.__set_Name("Ìì²Å");
	p.__set_UsrID(3);
	PersonAcs::Insert(p, sql);
	std::string serStr;
	int nSize;
	if (PersonAcs::SerializeToJSONString(p, &serStr, &nSize)) {
	    std::cout << serStr << std::endl;
	}
    }
    catch (std::exception & ex) {
	std::cout << ex.what() << std::endl;
    }
    std::cin.get();
    exit(0);
}
