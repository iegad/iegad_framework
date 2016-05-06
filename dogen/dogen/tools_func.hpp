#ifndef __TOOLS_FUNC__
#define __TOOLS_FUNC__


#include <string>
#include <algorithm>
#include <cctype>


#define BR_BR(out) { out <<"\n\n"; } 
#define TAB(out) (out << "\t", out)
#define TAB_TAB(out) (out << "\t\t", out) 
#define TAB_TAB_TAB(out) (out << "\t\t\t", out)
#define TAB_TAB_TAB_TAB(out) (out << "\t\t\t\t", out)
#define TAB_TAB_TAB_TAB_TAB(out) (out << "\t\t\t\t\t", out)
#define TAB_TAB_TAB_TAB_TAB_TAB(out) (out << "\t\t\t\t\t\t", out)

#define IFNDEF(str, out) { out << "#ifndef " <<std::string(str) << '\n'; }
#define DEFINE(str, out) { out << "#define " << std::string(str) << "\n\n\n"; }
#define INCLUDE(str, out) { out << "#include " << std::string(str) <<'\n'; }
#define ENDIF(str, out) { out << "#endif // " <<std::string(str) << '\n'; }

#define NAMESPACE_BEGIN(str, out) { out << "namespace " << std::string(str) << " {\n"; }
#define NAMESPACE_END(str, out) { out << "} // namespace " << std::string(str) << ";\n"; }

#define CLASS_BEGIN(str, out) { \
					out << "// =============================\n" \
					<< "// @" << std::string(str) << " ²Ù×÷¶ÔÏó\n" \
					<< "// =============================\n" \
					<< "\tclass " << std::string((str)) << "Acs {\n"; } 
#define CLASS_END(str, out) { out << "\t}; // " << std::string(str) << "Acs;\n\n\n"; }
#define PUBLIC(out) { out << "\tpublic:\n"; }
#define PROTECTED(out) { out << "\trprotected:\n"; }
#define PRIVATE(out) { out <<"\tprivate:\n" }

#define FUNCTION_SIGN(str, out) { out <<"\t\t"<<std::string(str)<<" {\n"; }
#define FUNCTION_END(str, out) { out <<"\t\t} // "<<std::string(str)<<";\n\n"; }


#define TRY_BEGIN(out) { out << "\t\t\ttry {\n"; }
#define TRY_END(out) { out << "\t\t\t}\n"; }
#define CATCH(out) { out << "\t\t\tcatch (std::exception & ex) {\n" \
									<< "\t\t\t\tif (errstr != nullptr) {\n" \
									<< "\t\t\t\t\t*errstr = ex.what();\n" \
									<< "\t\t\t\t}\n" \
									<< "\t\t\t} // catch;\n"; }


inline const std::string 
ToUpper(const std::string & src)
{
	std::string res = src;
	std::transform(res.begin(), res.end(), res.begin(), std::toupper);
	return res;
}


inline const std::vector<std::string>
SplitVct(const std::string &src, const std::string & chs)
{
	int pos = 0, len = chs.length(), n = 0, index = 0;
	std::vector<std::string> res;
	while (true) {
		index = src.find(chs, pos);
		if (index == -1) {
			res.push_back(std::string(src, pos, src.length() - pos));
			break;
		}
		n = index - n;
		res.push_back(std::string(src, pos, n));
		pos = index + len;
		n = pos;
	}
	return res;
}


inline bool
StartWith(const std::string & src, const std::string & substr)
{
	return src.find(substr, 0) == 0;
}


inline const std::string
ToLower(const std::string &src)
{
	std::string res = src;
	std::transform(res.begin(), res.end(), res.begin(), std::tolower);
	return res;
}


inline void 
ClearEmpty(std::vector<std::string> * strVct)
{
	for (auto itor = strVct->begin(); itor != strVct->end();) {
		if (itor->empty() || *itor == "") {
			itor = strVct->erase(itor);
		}
		else {
			++itor;
		}
	} // for;
}


inline const std::string
Trim(const std::string &src, char chr)
{
	std::string restr;
	for (int i = 0, n = src.length(); i < n; i++) {
		if (src[i] == chr) {
			continue;
		}
		restr.push_back(src[i]);
	}
	return restr;
}


inline const std::string
Replace(const std::string &src, const std::string & oldstr, const std::string & newstr)
{
	int pos = 0, len = oldstr.length();
	std::string restr(src);
	while (true) {
		pos = restr.find(oldstr, pos);
		if (pos == -1) {
			break;
		}
		restr.replace(pos, len, newstr);
		pos++;
	}
	return restr;
}


#endif // __TOOLS_FUNC__