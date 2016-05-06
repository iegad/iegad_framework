#include "SQLGenerater.h"
#include "tools_func.hpp"


bool 
dogen::SQLGenerater::_classInfo(const data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	do {
		if (!_tabHead(thriftClass, fout)) {
			break;
		}

		if (!_addTabFiled(thriftClass, fout)) {
			break;
		}

		if (!_tabTail(thriftClass, fout)) {
			break;
		}

		return true;
	} while (false);
	return false;
}


bool 
dogen::SQLGenerater::_tabHead(const data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	fout << "CREATE TABLE " << ToUpper(thriftClass.GetClassName()) << "_T (\n";
	return !fout.fail();
}


bool 
dogen::SQLGenerater::_tabTail(const data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	fout << ") ENGINE = INNODB CHARACTER SET utf8 COLLATE utf8_general_ci;\n\n\n";
	return !fout.fail();
}


bool 
dogen::SQLGenerater::_addTabFiled(const data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	const data_structure::ThriftClass::ThriftClassMemberMap_t & memMap = thriftClass.GetClassMember();
	for (auto itor = memMap.begin(); itor != memMap.end(); itor++) {
		fout << ToUpper(itor->second.MemberName) << "\t\t" << _getType(itor->second.MemberType) << ",\n";
	}
	fout << "PRIMARY KEY " << ToUpper(thriftClass.GetClassName()) << "_PK (";
	const std::vector<std::string> & keys = thriftClass.GetKeys();
	for (int i = 0, n = keys.size(); i < n; i++) {
		fout << ToUpper(keys[i]);
		if (i + 1 != n) {
			fout << ", ";
		}
	}
	fout << ")\n";
	return !fout.fail();
}


const std::string 
dogen::SQLGenerater::_getType(const std::string & typeStr)
{
	if (typeStr == "std::string") {
		return "VARCHAR(200)";
	}
	if (typeStr == "int64_t") {
		return "BIGINT";
	}
	if (typeStr == "int32_t") {
		return "INTEGER";
	}
	if (typeStr == "int16_t") {
		return "SMALLINT";
	}
	if (typeStr == "int8_t" || typeStr == "bool") {
		return "TINYINT";
	}
	if (typeStr == "double") {
		return "DOUBLE";
	}	
}


bool 
dogen::SQLGenerater::_statment(std::ofstream & fout)
{
	fout << "/* ================================ \n";
	fout << " 该文件由dogen 生成, 可能需要手动修改部分内容\n";
	fout << "\t1 , VARCHAR(200) 为默认, 如需要其他长度, 请自行修改\n";
	fout << "\t2, 如果 生成的主键与业务需求不同, 请手动修改\n";
	fout << "\t3, 该生成器不会自动生成索引, 如需要使用索引, 请自行修改\n";
	fout << "\t4, 默认会生成INNODB引擎, 字符集为utf8, 简体中文, 如与需求不同, 请自行修改\n";
	fout << " ================================ */\n\n\n\n";
	return !fout.fail();
}
