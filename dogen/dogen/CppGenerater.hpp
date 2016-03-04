#ifndef __CPP_GENERATER__
#define __CPP_GENERATER__


#include "IGenerater.hpp"
#include <stdint.h>
#include <string>
#include "IDL_FileInfo.hpp"
#include <algorithm>
#include <cctype>


class CppGenerater : public IGenerater {
public:
    CppGenerater() {}

    virtual ~CppGenerater() {}


    virtual bool Generate(const FileInfo & fileInfo)
    {
	std::ofstream fout;
	try {	    
	    do {
		fout.open(fileInfo.FileName(), std::ios_base::out);
		if (fout.fail()) {
		    break;
		}

		fout << Ifndef() << Define() << Include() << Statement();
		fout << NameSpaceBegin(fileInfo);

		for (auto itor = fileInfo.GetClassMap().begin();
		    itor != fileInfo.GetClassMap().end();
		    itor++) {
		    OutClassInfo(itor->second, fout);
		} // for(;;);

		fout << NameSpaceEnd(fileInfo);
		fout << Endif();
		fout.close();
		return true;
	    } while (false);
	}
	catch (std::exception &) {
	    if (fout.is_open()) {
		fout.close();
	    }	    
	} // catch (std::exception &);
	return false;
    }
    

private:

    // ============== class info ====================
    void OutClassInfo(const ClassInfo & classInfo, std::ofstream & fout) {
	fout << "\r\n    class " << classInfo.GetName() << " {\n";

	// 添加构造函数
	fout << "    public:\n";
	OutCtorInfo(classInfo, fout);

	// 添加数据库 增, 删, 改, 查 函数
	OutInsertFun(classInfo, fout);

	// 添加字段
	fout << "    private:\n";
	for (auto itor = classInfo.ClassMembers().begin(); 
	    itor != classInfo.ClassMembers().end(); 
	    itor++) {
	    OutMemberInfo(itor->second, fout);
	} // for;

	fout << "    }; // class " << classInfo.GetName() << ";\r\n";
    }


    void OutInsertFun(const ClassInfo & classInfo, std::ofstream & fout) {
	std::string temp = classInfo.GetName();
	std::transform(temp.begin(), temp.end(), temp.begin(), std::toupper);

	// 函数名称
	fout << "\tbool Insert(soci::session * sql, std::string * errstr = nullptr) {\n";

	fout << "\t\ttry {\n\t";

	// INSERT INTO 
	fout << "\t\t*sql << \"INSERT INTO " << temp << "_T \"\n\t\t\t\"(";
	int nCount = 1;
	for (auto itor = classInfo.ClassMembers().begin();
	    itor != classInfo.ClassMembers().end();
	    itor++, nCount++) {
	    if (nCount != 1 && (nCount - 1) % 3 == 0) {
		fout << "\"\n\t\t\t\"";
	    }
	    temp = itor->second.MemberName;
	    std::transform(temp.begin(), temp.end(), temp.begin(), std::toupper);
	    fout << temp;
	    if (nCount != classInfo.ClassMembers().size()) {
		fout << ", ";
	    }
	} // for;

	// VALUES
	nCount = 1;
	fout << ")\"\n\t\t\t\" VALUES\"\n\t\t\t\"(";
	for (auto itor = classInfo.ClassMembers().begin();
	    itor != classInfo.ClassMembers().end();
	    itor++, nCount++) {
	    if (nCount != 1 && (nCount - 1) % 3 == 0) {
		fout << "\"\n\t\t\t\"";
	    }
	    temp = itor->second.MemberName;
	    std::transform(temp.begin(), temp.end(), temp.begin(), std::toupper);
	    fout << ':' << temp;
	    if (nCount != classInfo.ClassMembers().size()) {
		fout << ", ";
	    }
	} // for;
	fout << ")\", \n\t\t\t";
	
	nCount = 1;
	// soci::use()
	for (auto itor = classInfo.ClassMembers().begin();
	    itor != classInfo.ClassMembers().end();
	    itor++, nCount++) {
	    if (nCount != 1 && (nCount - 1) % 3 == 0) {
		fout << "\n\t\t\t";
	    }
	    temp = itor->second.MemberName;
	    fout << "soci::use(" << temp << "_)";
	    if (nCount != classInfo.ClassMembers().size()) {
		fout << ", ";
	    }
	} // for;


	fout << ";\n\t\t\treturn true;\n\t\t}\n";
	fout << "\t\tcatch (std::exception & ex) {\n\t\t\t";
	fout << "if (errstr != nullptr) {\n\t\t\t\t*errstr = ex.what();\n\t\t\t}\n\t\t}\n";
	fout << "\t\treturn false;\n\t}\r\n";
    }


    void OutCtorInfo(const ClassInfo & classInfo, std::ofstream & fout) {
	fout << "\t// 构造函数\n\t" << classInfo.GetName() << "() : \n";
	int nCount = 1;
	for (auto itor = classInfo.ClassMembers().begin();
	    itor != classInfo.ClassMembers().end();
	    itor++, nCount++) {
	    OutCtor_MemberInfo(itor->second, fout);
	    if (nCount != classInfo.ClassMembers().size()) {
		fout << ",";
	    }
	    fout << "\n";
	} // for;
	fout << "\t{}\r\n";
    }


    void OutCtor_MemberInfo(const ClassMember & mem, std::ofstream & fout) {
	fout << "\t    " << mem.MemberName << "_(" << mem.DefaultValue << ")";
    }


    void OutMemberInfo(const ClassMember & mem, std::ofstream & fout) {
	std::string typeStr;
	switch (mem.MemberType) {
	case MemberTypeE::CHAR: typeStr = "char";  
	    break;
	case MemberTypeE::INT8: typeStr = "int8_t";  
	    break;
	case MemberTypeE::INT16: typeStr = "int16_t";  
	    break;
	case MemberTypeE::INT32: typeStr = "int32_t";  
	    break;
	case MemberTypeE::INT64: typeStr = "int64_t";  
	    break;
	case MemberTypeE::FLOAT: typeStr = "float";  
	    break;
	case MemberTypeE::DOUBLE: typeStr = "double";  
	    break;
	case MemberTypeE::STRING: typeStr = "std::string";  
	    break;
	}
	fout << "\t" << typeStr << ' ' << mem.MemberName << "_;\n";
    }
    // ============== class info ====================


    // ================ namespace ==================
    const std::string NameSpaceBegin(const FileInfo & fileInfo) {
	if (fileInfo.GetNameSpaceInfo().GetName() == "") {
	    return "";
	}
	return "\r\nnamespace " + fileInfo.GetNameSpaceInfo().GetName() + " {\n";
    }

    const std::string NameSpaceEnd(const FileInfo & fileInfo) {
	if (fileInfo.GetNameSpaceInfo().GetName() == "") {
	    return "";
	}
	return "\n} // namespace " + fileInfo.GetNameSpaceInfo().GetName() + ";\n";
    }
    // ================ namespace ==================

    const std::string Ifndef() {
	return "#ifndef __DOGEN_CPP_GENERATER__\n";
    }

    const std::string Define() {
	return "#define __DOGEN_CPP_GENERATER__\r\n";
    }

    const std::string Statement() {
	return "/* =====================\n"
		    "\t\t该文件由dogen生成\n"
		    "\t\t@作者 : iegad\n "
		    "===================== */\n";
    }

    const std::string Endif() {
	return "\r\n#endif // __DOGEN_CPP_GENERATER__\n";
    }

    const std::string Include() {
	return "\n#include <string>\n"
		    "#include <stdint.h>\n"
		    "#include <soci.h>\r\r\n";
    }

}; // class CppGenerater;


#endif // __CPP_GENERATER__