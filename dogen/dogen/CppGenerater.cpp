#include "CppGenerater.h"
#include "tools_func.hpp"


bool 
dogen::CppGenerater::_head(std::ofstream & fout)
{
	IFNDEF("__DATA_OBJ_CPP_GENERATER__", fout);
	DEFINE("__DATA_OBJ_CPP_GENERATER__", fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_include(std::ofstream & fout)
{
	INCLUDE("<string>", fout);
	INCLUDE("<list>", fout);
	INCLUDE("<stdint.h>", fout);
	INCLUDE("<soci/soci.h>", fout);
	INCLUDE("\"thrift_ex/thrift_serializer.hpp\"", fout);
	BR_BR(fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_tail(std::ofstream & fout)
{
	ENDIF("__DATA_OBJ_CPP_GENERATER__", fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_namespaceBegin(const ::data_structure::ThriftNamespace & thriftNamespace, std::ofstream & fout)
{
	const std::vector<std::string> & nsVct = thriftNamespace.GetNamespaces();
	for (int i = 0, n = nsVct.size(); i < n; i++) {
		NAMESPACE_BEGIN(nsVct[i], fout);
		if (i + 1 == n) {
			BR_BR(fout);
		}
	} // for (int i = 0, n = nsVct.size(); i < n; i++);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_namespaceEnd(const ::data_structure::ThriftNamespace & thriftNamespace, std::ofstream & fout)
{
	const std::vector<std::string> & nsVct = thriftNamespace.GetNamespaces();
	for (int i = nsVct.size() - 1, n = 0; i >= n; i--) {
		NAMESPACE_END(nsVct[i], fout);
	} // for (int i = nsVct.size() - 1, n = 0; i >= n; i--);
	BR_BR(fout);
	return !fout.fail();
}


bool
dogen::CppGenerater::_classInfo(const data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	do {
		if (!this->_classHead(thriftClass, fout)) {
			break;
		}

		PUBLIC(fout);

		if (!this->_classInsertFunc(thriftClass, fout)) {
			break;
		}

		if (!this->_classDeleteFunc(thriftClass, fout)) {
			break;
		}

		if (!this->_classUpdateFunc(thriftClass, fout)) {
			break;
		}

		if (!this->_classSelectAllFunc(thriftClass, fout)) {
			break;
		}

		if (!this->_classSerializeToJSONStringFunc(thriftClass, fout)) {
			break;
		}

		if (!this->_classParseFromJSONStringFunc(thriftClass, fout)) {

		}

		if (!this->_classSerializeToArrayFunc(thriftClass, fout)) {
			break;
		}

		if (!this->_classParseFromArrayFunc(thriftClass, fout)) {
			break;
		}

		if (!this->_classTail(thriftClass, fout)) {
			break;
		}
		return !fout.fail();
	} while (false);
	return false;
}


bool 
dogen::CppGenerater::_classHead(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	CLASS_BEGIN(thriftClass.GetClassName(), fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classTail(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	CLASS_END(thriftClass.GetClassName(), fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classInsertFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	FUNCTION_SIGN("static bool Insert(const " + thriftClass.GetClassName() 
		+ " & obj, soci::session & sql, std::string * errstr = nullptr)", fout);
	TRY_BEGIN(fout);
	TAB_TAB_TAB_TAB(fout) << "sql << \"INSERT INTO " << ToUpper(thriftClass.GetClassName()) << "_T \"\n";

	if (!this->_classAddInsertKeyParam(thriftClass, fout)) {
		return false;
	}

	TAB_TAB_TAB_TAB_TAB(fout) << "\"VALUES\"\n";

	if (!this->_classAddInsertSignedParam(thriftClass, fout)) {
		return false;
	}

	TAB_TAB_TAB_TAB_TAB(fout);
	if (!this->_classAddInsertValueParam(thriftClass, fout)) {
		return false;
	}	
	fout << ";\n";
	TAB_TAB_TAB_TAB(fout) << "return true;\n";
	TRY_END(fout);
	CATCH(fout);
	TAB_TAB_TAB(fout) << "return false;\n";
	FUNCTION_END("static bool Insert", fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classUpdateFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	FUNCTION_SIGN("static bool Update(const " + thriftClass.GetClassName()
		+ " & obj, soci::session & sql, std::string * errstr = nullptr)", fout);

	TRY_BEGIN(fout);
	TAB_TAB_TAB_TAB(fout) << "sql << \"UPDATE " << ToUpper(thriftClass.GetClassName()) << "_T SET \"\n";

	if (!this->_classAddUpdateSignedParam(thriftClass, fout)) {
		return false;
	}

	TAB_TAB_TAB_TAB_TAB(fout) << "\"WHERE \"\n";

	if (!this->_classAddWhereSignedParam(thriftClass, fout)) {
		return false;
	}

	if (!this->_classAddUpdateValueParam(thriftClass, fout)) {
		return false;
	}	

	if (!this->_classAddWhereValueParam(thriftClass, fout)) {
		return false;
	}

	TAB_TAB_TAB_TAB(fout) << "return true;\n";
	TRY_END(fout);
	CATCH(fout);
	TAB_TAB_TAB(fout) << "return false;\n";
	FUNCTION_END("static bool Update", fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classDeleteFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	FUNCTION_SIGN("static bool Delete(const " + thriftClass.GetClassName()
		+ " & obj, soci::session & sql, std::string * errstr = nullptr)", fout);
	TRY_BEGIN(fout);

	TAB_TAB_TAB_TAB(fout) << "sql << \"DELETE * FROM " << ToUpper(thriftClass.GetClassName()) << "_T WHERE \"\n";
	if (!this->_classAddWhereSignedParam(thriftClass, fout)) {
		return false;
	}

	if (!this->_classAddWhereValueParam(thriftClass, fout)) {
		return false;
	}

	TAB_TAB_TAB_TAB(fout) << "return true;\n";
	TRY_END(fout);
	CATCH(fout);
	TAB_TAB_TAB(fout) << "return false;\n";
	FUNCTION_END("static bool Delete", fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classSelectAllFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	FUNCTION_SIGN("static bool SelectAll(std::list<" + thriftClass.GetClassName()
		+ "> * dataList, soci::session & sql, std::string * errstr = nullptr)", fout);

	TRY_BEGIN(fout);

	TAB_TAB_TAB_TAB(fout) << "soci::rowset<soci::row> rows = (sql.prepare << \"SELECT * FROM "
		<< ToUpper(thriftClass.GetClassName()) << "_T\");\n";

	TAB_TAB_TAB_TAB(fout) << "for (auto itor = rows.begin(); itor != rows.end(); itor++) {\n";

	if (!this->_classAddSelectValueParam(thriftClass, fout)) {
		return false;
	}
	TAB_TAB_TAB_TAB(fout) << "} // for;\n";
	TAB_TAB_TAB_TAB(fout) << "return true;\n";
	TRY_END(fout);
	CATCH(fout);
	TAB_TAB_TAB(fout) << "return false;\n";
	FUNCTION_END("static bool SelectAll", fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classSerializeToJSONStringFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	FUNCTION_SIGN("static bool SerializeToJSONString(" + thriftClass.GetClassName() 
		+ " & obj, std::string * serstr, int * size)", fout);

	TAB_TAB_TAB(fout) << "return iegad::thrift_ex::Serializer<" << thriftClass.GetClassName() 
		<< ">::SerializeToJSONString(obj, serstr, size);\n";
	FUNCTION_END("static bool SerializeToJSONString", fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classParseFromJSONStringFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	FUNCTION_SIGN("static bool ParseFromJSONString(" + thriftClass.GetClassName()
		+ " * obj, const std::string & serstr)", fout);

	TAB_TAB_TAB(fout) << "return iegad::thrift_ex::Serializer<" << thriftClass.GetClassName() 
		<< ">::ParseFromJSONString(obj, serstr);\n";
	FUNCTION_END("static bool ParseFromJSONString", fout);
	return !fout.fail();
}


bool
dogen::CppGenerater::_classSerializeToArrayFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	FUNCTION_SIGN("static bool SerializeToArray(" + thriftClass.GetClassName() 
		+ " & obj, std::string * serstr, int * size)", fout);

	TAB_TAB_TAB(fout) << "return iegad::thrift_ex::Serializer<" 
		<< thriftClass.GetClassName() 
		<< ">::SerializeToArray(obj, serstr, size);\n";
	FUNCTION_END("static bool SerializeToArray", fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classParseFromArrayFunc(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	FUNCTION_SIGN("static bool ParseFromArray(" + thriftClass.GetClassName()
		+ " * obj, const std::string & serstr)", fout);

	TAB_TAB_TAB(fout) << "return iegad::thrift_ex::Serializer<" 
		<< thriftClass.GetClassName() 
		<< ">::ParseFromArray(obj, serstr);\n";
	FUNCTION_END("static bool ParseFromArray", fout);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classAddInsertKeyParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	data_structure::ThriftClass::ThriftClassMemberMap_t memMap = thriftClass.GetClassMember();
	int n = 0;
	TAB_TAB_TAB_TAB_TAB(fout) << "\"(";
	for (auto itor = memMap.begin(); itor != memMap.end(); itor++, n++) {
		if (n % 3 == 0 && n != 0) {
			fout << "\"\n";
			TAB_TAB_TAB_TAB_TAB(fout) << "\"";
		}
		fout << ToUpper(itor->second.MemberName);
		if (n + 1 != memMap.size()) {
			fout << ", ";
		}
	}
	fout << ") \"\n";
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classAddInsertSignedParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	data_structure::ThriftClass::ThriftClassMemberMap_t memMap = thriftClass.GetClassMember();
	int n = 0;
	TAB_TAB_TAB_TAB_TAB(fout) << "\"(";
	for (auto itor = memMap.begin(); itor != memMap.end(); itor++, n++) {
		if (n % 3 == 0 && n != 0) {
			fout << "\"\n";
			TAB_TAB_TAB_TAB_TAB(fout) << "\"";
		}
		fout << ':' <<ToUpper(itor->second.MemberName);
		if (n + 1 != memMap.size()) {
			fout << ", ";
		}
	}
	fout << ")\",\n";
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classAddInsertValueParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	data_structure::ThriftClass::ThriftClassMemberMap_t memMap = thriftClass.GetClassMember();
	int n = 0;
	for (auto itor = memMap.begin(); itor != memMap.end(); itor++, n++) {
		if (n % 3 == 0 && n != 0) {
			fout << '\n';
			TAB_TAB_TAB_TAB_TAB(fout);

		}
		fout << "soci::use(obj." << itor->second.MemberName << ')';
		if (n + 1 != memMap.size()) {
			fout << ", ";
		}
	}
	return !fout.fail();
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classAddSelectValueParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	const ::data_structure::ThriftClass::ThriftClassMemberMap_t & memMap = thriftClass.GetClassMember();
	TAB_TAB_TAB_TAB_TAB(fout) << thriftClass.GetClassName() << " obj;\n";
	for (auto itor = memMap.begin(); itor != memMap.end(); itor++) {		
		TAB_TAB_TAB_TAB_TAB(fout) << "obj.__set_" << itor->second.MemberName
			<< "(itor->get<" << itor->second.MemberType << ">(\""
			<< ToUpper(itor->second.MemberName) << "\", ";
		if (itor->second.MemberType == "std::string") {
			fout << '\"';
		}
		fout << itor->second.MemberDefaultValue;
		if (itor->second.MemberType == "std::string") {
			fout << '\"';
		}
		fout << "));\n";
	}
	TAB_TAB_TAB_TAB_TAB(fout) <<"dataList->push_back(obj);\n";
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classAddUpdateSignedParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	::data_structure::ThriftClass::ThriftClassMemberMap_t memMap = thriftClass.GetClassMember();
	const std::vector<std::string> & keys = thriftClass.GetKeys();

	for (int i = 0, n = keys.size(); i < n; i++) {
		auto findItor = memMap.find(keys[i]);
		if (findItor != memMap.end()) {
			memMap.erase(findItor);
		}
	} // for;

	int n = 1;
	for (auto itor = memMap.begin(); itor != memMap.end(); itor++, n++) {
		TAB_TAB_TAB_TAB_TAB(fout)
			<< "\"" << ToUpper(itor->second.MemberName) 
			<< " = :" << ToUpper(itor->second.MemberName);
		if (n != memMap.size()) {
			fout << ", \"\n";
		}		
	} // for;
	fout << "\", \n";
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classAddUpdateValueParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	::data_structure::ThriftClass::ThriftClassMemberMap_t memMap = thriftClass.GetClassMember();
	const std::vector<std::string> & keys = thriftClass.GetKeys();

	for (int i = 0, n = keys.size(); i < n; i++) {
		auto findItor = memMap.find(keys[i]);
		if (findItor != memMap.end()) {
			memMap.erase(findItor);
		}
	} // for;
	int n = 0;
	TAB_TAB_TAB_TAB_TAB(fout);
	for (auto itor = memMap.begin(); itor != memMap.end(); itor++, n++) {
		if (n % 3 == 0 && n != 0) {
			fout << '\n';
			TAB_TAB_TAB_TAB_TAB(fout);
		}
		fout << "soci::use(obj." << itor->second.MemberName << ')';
		if (n + 1 != memMap.size()) {
			fout << ", ";
		}
	}
	fout << ",\n";
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classAddWhereValueParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	const std::vector<std::string> & keys = thriftClass.GetKeys();
	int i = 0;
	if (keys.size() > 0) {
		for (int n = keys.size() - 1; i < n; i++) {
			TAB_TAB_TAB_TAB_TAB(fout) << "soci::use(obj." << keys[i] << "), \n";
		}
		TAB_TAB_TAB_TAB_TAB(fout) << "soci::use(obj." << keys[i] << ");\n";
	} // if (keys.size() > 0);
	return !fout.fail();
}


bool 
dogen::CppGenerater::_classAddWhereSignedParam(const ::data_structure::ThriftClass & thriftClass, std::ofstream & fout)
{
	const std::vector<std::string> & keys = thriftClass.GetKeys();
	int i = 0;
	if (keys.size() > 0) {
		for (int n = keys.size() - 1; i < n; i++) {
			TAB_TAB_TAB_TAB_TAB(fout) << "\"" << ToUpper(keys[i]) << " = :" << ToUpper(keys[i]) << " AND \"\n";
		}
		TAB_TAB_TAB_TAB_TAB(fout) << "\"" << ToUpper(keys[i]) << " = :" << ToUpper(keys[i]) << "\",\n";
	} // if (keys.size() > 0);
		
	return !fout.fail();	
}
