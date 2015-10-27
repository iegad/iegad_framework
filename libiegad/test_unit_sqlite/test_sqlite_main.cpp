#include <iostream>
#include "sqlite/iegad_sqlite.h"
#include <string>


#ifdef WIN32
#define FILENAME    ".\\test.db"
#else
#define FILENAME    "./test.db"
#endif // WIN32


std::string CREATE_TAB_SQL = "CREATE TABLE TEST_TAB (T_ID INT, T_VAL VARCHAR(255))";


int
main(int argc, char * argv[])
{
    iegad::sqlite::sqlite_helper db_handler;
    do {
	if (db_handler.open(FILENAME) != 0) {
	    break;
	}

	if (db_handler.exec(CREATE_TAB_SQL) != 0) {
	    break;
	}

	std::vector<std::string> sqlvct;
	char c = 'a';
	for (int i = 0; i < 20; i++) {
	    sqlvct.push_back(std::move(std::string("INSERT INTO TEST_TAB (T_ID, T_VAL) VALUES (" + std::to_string(i) + ", '" + std::to_string((char)c++) + "')")));
	}

	if (db_handler.exec_trans(sqlvct) < 0) {
	    break;
	}

	iegad::data::db_tab tab;
	if (db_handler.query("SELECT * FROM TEST_TAB", tab) != 0) {
	    break;
	}

	for (int i = 0, n = tab.row_count(); i < n; i++) {
	    for (int j = 0, m = tab[i].col_count(); j < m; j++) {
		std::cout << tab[i][j] << "\t";
	    }
	    std::cout << "\n";
	}
	

    } while (false);
    std::cout << db_handler.last_errmsg() << std::endl;
    std::cin.get();


    exit(0);
}