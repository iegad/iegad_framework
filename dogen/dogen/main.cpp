#include <iostream>
#include "IDL_FileInfo.hpp"
#include "CppGenerater.hpp"


int
main(int argc, char * argv[])
{
    if (argc < 3) {
	std::cout << "´íÎóµÄ²ÎÊý" << std::endl;
	exit(1);
    }
    FileInfo file(argv[1], argv[2]);
    CppGenerater gen;
    NamespaceInfo ni;
    ni.SetName("static_data");
    file.SetNameSapceInfo(ni);
    ClassInfo classInfo;
    std::vector<std::string> primaryVct;
    primaryVct.push_back("name");
    classInfo.SetName("Employee");
    classInfo.AddMember("age", MemberTypeE::INT32, "0");
    classInfo.AddMember("name", MemberTypeE::STRING, "");
    classInfo.AddMember("idcard", MemberTypeE::STRING, "");
    classInfo.AddMember("nation", MemberTypeE::STRING, "");
    classInfo.AddMember("sex", MemberTypeE::INT16, "0");
    classInfo.AddMember("job", MemberTypeE::STRING, "");
    classInfo.SetPrimary(primaryVct);
    file.AddClass(classInfo);
    gen.Generate(file);
    exit(0);
}