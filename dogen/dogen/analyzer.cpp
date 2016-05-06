#include "analyzer.h"
#include <fstream>
#include "tools_func.hpp"


bool 
dogen::Analyzer::Analyze(data_structure::ThriftFile * thriftFile, std::string * errstr)
{
	std::ifstream fin;
	std::string lineStr;
	std::string className;
	int flag = false;
	try {
		do {
			fin.open(inFilename_, std::ios_base::in);
			if (fin.fail()) {
				break;
			}
			while (!fin.eof()) {
				std::getline(fin, lineStr);
				if (StartWith(ToLower(lineStr), "namespace") && _addNameSpace(lineStr, thriftFile) < 0) {
					*errstr += "ÃüÃû¿Õ¼ä ÉùÃ÷´íÎó";
					break;
				} 
				if (StartWith(ToLower(lineStr), "struct")) {
					flag = _addClass(lineStr, thriftFile, &className);
					if (flag > 0) {
						std::getline(fin, lineStr, '}');
						if (_addMembers(lineStr, className, thriftFile) < 0) {
							break;
						}
					}
					else if (flag == 0) {
						continue;
					}
					else {
						*errstr += "Àà ÉùÃ÷´íÎó";
						break;
					}
				} // if (StartWith(ToLower(lineStr), "struct"));
			} // while (fin.eof());

			if (!fin.eof() || !errstr->empty()) {
				break;
			}

			fin.close();
			return true;
		} while (false);
	}
	catch (std::exception & ex) {
		if (fin.is_open()) {
			fin.close();
		}
		if (errstr != nullptr) {
			*errstr += ex.what();
		}
	} // catch;
	return false;
}


int 
dogen::Analyzer::_addNameSpace(const std::string & line, data_structure::ThriftFile * thriftFile)
{
	do {
		std::vector<std::string> strVct = SplitVct(line, " ");
		if (strVct.size() != 3) {
			break;
		}

		if (strVct[1] != "cpp") {
			return 0;
		}

		std::vector<std::string> nsLevel = SplitVct(strVct[2], ".");
		for (int i = 0, n = nsLevel.size(); i < n; i++) {
			if (!thriftFile->AddNamespaceLevel(nsLevel[i])) {
				return -1;
			}
		} // for;
		return 1;
	} while (false);
	return -1;
}


int
dogen::Analyzer::_addClass(const std::string & line, data_structure::ThriftFile * thriftFile, std::string * className)
{
	do {
		std::vector<std::string> strVct = SplitVct(line, " ");
		if (strVct.size() < 2) {
			break;
		}
		std::string cName = strVct[1];
		data_structure::ThriftClass tc;
		tc.SetClassName(cName);
		*className = cName;
		thriftFile->AddClass(tc);
		return 1;
	} while (false);
	return -1;
}


int
dogen::Analyzer::_addMembers(const std::string & line, const std::string & className, data_structure::ThriftFile * thriftFile)
{
	data_structure::ThriftClassMember tcm;
	std::vector<std::string> strVct = SplitVct(line, ";");
	std::map<std::string, data_structure::ThriftClass> & cmap = thriftFile->GetClassMap();
	auto itor = cmap.find(className);

	if (itor == cmap.end()) {
		return -1;
	}

	std::string typeStr;
	do {
		if (strVct.size() == 0) {
			break;
		}

		for (int i = 0, n = strVct.size() - 1; i < n; i++) {

			std::vector<std::string> mInfoVct = SplitVct(Trim(strVct[i], '\n'), " ");
			ClearEmpty(&mInfoVct);
			if (mInfoVct.size() < 4) {
				continue;
			}
			tcm.MemberName = mInfoVct[3];

			if (mInfoVct[1] == "required") {
				itor->second.AddKey(mInfoVct[3]);
			}

			if (!_getTypeString(mInfoVct[2], &typeStr)) {
				return -1;
			}

			tcm.MemberType = typeStr;
			if (mInfoVct.size() == 6) {
				tcm.MemberDefaultValue = mInfoVct[5];
			}

			itor->second.AddClassMember(tcm.MemberName, tcm.MemberType, tcm.MemberDefaultValue);
			tcm.Clear();
			
		} // for;
		return 1;
	} while (false);
	return -1;
}


bool 
dogen::Analyzer::_getTypeString(const std::string & srcTypeStr, std::string * typeStr)
{
	if (srcTypeStr == "byte") {
		*typeStr = "int8_t";
	}
	else if (srcTypeStr == "i16") {
		*typeStr = "int16_t";
	}
	else if (srcTypeStr == "i32") {
		*typeStr = "int32_t";
	}
	else if (srcTypeStr == "i64") {
		*typeStr = "int64_t";
	}
	else if (srcTypeStr == "bool") {
		*typeStr = "bool";
	}
	else if (srcTypeStr == "string") {
		*typeStr = "std::string";
	}
	else if (srcTypeStr == "double") {
		*typeStr = "double";
	}
	else {
		return false;
	}
	return true;
}
