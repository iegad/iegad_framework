#include "iegad_string.h"
#include <string.h>
#include <sstream>
#include <codecvt>



const std::vector<std::string>
iegad::string::split_vct(const std::string &src, const std::string chs)
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


const std::string
iegad::string::substr(const std::string &src, int pos, int n)
{
    std::string restr(src, pos, n);
    return restr;
}


const std::string
iegad::string::trim(const std::string &src)
{
    int n = src.length() - 1;
    while (isspace(src[n])) {
	n--;
    }
    std::string restr(src, 0, n + 1);
    return restr;
}


const std::string
iegad::string::trim(const std::string &src, char chr)
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


const std::string
iegad::string::ltrim(const std::string &src)
{
    int rpos = 0;
    while (isspace(src[rpos]))
        rpos++;
    std::string restr(src, rpos, src.length() - rpos);
    return restr;
}


const std::string
iegad::string::replace(const std::string &src, const std::string oldstr, const std::string newstr)
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


int
iegad::string::fstchr(const std::string &src, char chr)
{
    for (int i = 0, n = src.length(); i < n; i++) {
        if (src[i] == chr)
            return i;
    }
    return -1;
}


int
iegad::string::lstchr(const std::string &src, char chr)
{
    for (int i = src.length() - 1; i >= 0; i--) {
        if (src[i] == chr)
            return i;
    }
    return -1;
}


const std::string
iegad::string::to_upr(const std::string &src)
{
    std::string restr(src);
    for (int i = 0, n = restr.length(); i < n; i++)
        restr[i] = toupper(restr[i]);
    return restr;
}


const std::string
iegad::string::to_lwr(const std::string &src)
{
    std::string restr(src);
    for (int i = 0, n = restr.length(); i < n; i++)
        restr[i] = tolower(restr[i]);
    return restr;
}


const std::string 
iegad::string::to_str(int val)
{
    return to_str<int>(val);
}


const std::string
iegad::string::to_str(short val)
{
    return to_str<short>(val);
}

const std::string iegad::string::to_str(long val)
{
    return to_str<long>(val);
}


const std::string 
iegad::string::to_str(long long val)
{
    return to_str<long long>(val);
}


const std::string
iegad::string::to_str(unsigned int val)
{
    return to_str<unsigned int>(val);
}


const std::string 
iegad::string::to_str(unsigned short val)
{
    return to_str<unsigned short>(val);
}


const std::string 
iegad::string::to_str(unsigned long val)
{
    return to_str<unsigned long>(val);
}


const std::string 
iegad::string::to_str(unsigned long long val)
{
    return to_str<unsigned long long>(val);
}


const std::string 
iegad::string::to_str(float val)
{
    std::string res;
    std::stringstream strm;
    strm.precision(8);
    strm << val;
    strm >> res;
    return res;
}


const std::string
iegad::string::to_str(double val)
{
    std::string res;
    std::stringstream strm;
    strm.precision(11);
    strm << val;
    strm >> res;
    return res;
}


const std::string 
iegad::string::to_str(bool val)
{
    return val ? "true" : "false";
}


const std::string 
iegad::string::to_str(long double val)
{
    std::string res;
    std::stringstream strm;
#ifdef WIN32
    strm.precision(11);
#else
    strm.precision(18);
#endif // WIN32
    strm << val;
    strm >> res;
    return res;
}


const std::string 
iegad::string::unicode_to_utf8(const std::wstring & val)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.to_bytes(val);
}


const std::wstring 
iegad::string::utf8_to_unicode(const std::string & val)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(val);
}


/******************************************************/


