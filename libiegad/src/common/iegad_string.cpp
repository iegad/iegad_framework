#include "iegad_string.h"
#include <string.h>
#include <sstream>
#include <codecvt>
#include <iomanip>
#include <boost/uuid/sha1.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>



const std::vector<std::string>
iegad::string::split_vct(const std::string &src, const std::string & chs)
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
iegad::string::substr(const std::string &src, unsigned int pos, int n /*=-1*/)
{
    int len = n < 0 ? src.size() - pos : n;
    std::string restr(src, pos, len);
    return restr;
}


const std::string
iegad::string::rtrim(const std::string &src)
{
    int n = src.length() - 1;
    while (std::isspace(src[n])) {
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
iegad::string::trim(const std::string & src)
{
    std::string restr;
    for (int i = 0, n = src.length(); i < n; i++) {
	if (std::isspace(src[i])) {
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
    while (std::isspace(src[rpos]))
        rpos++;
    std::string restr(src, rpos, src.length() - rpos);
    return restr;
}


const std::string
iegad::string::replace(const std::string &src, const std::string & oldstr, const std::string & newstr)
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
iegad::string::to_str(float val, int p /* = 6*/)
{
    std::string res;
    std::stringstream strm;
    strm.precision(p);
    strm << val;
    strm >> res;
    return res;
}


const std::string
iegad::string::to_str(double val, int p /* = 15*/)
{
    std::string res;
    std::stringstream strm;
    strm.precision(p);
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
iegad::string::to_str(long double val, int p /* = 15*/)
{
    std::string res;
    std::stringstream strm;
    strm.precision(p);
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


char 
iegad::string::to_char(const std::string & str)
{
    return str[0];
}


short 
iegad::string::to_int16(const std::string & str)
{
    std::stringstream ss;
    short res;
    ss.str(str);
    ss >> res;  
    return res;
}


unsigned short 
iegad::string::to_uint16(const std::string & str)
{
    std::stringstream ss;
    unsigned short res;
    ss.str(str);
    ss >> res;
    return res;
}


int 
iegad::string::to_int32(const std::string & str)
{
    std::stringstream ss;
    int res;
    ss.str(str);
    ss >> res;
    return res;
}


unsigned int 
iegad::string::to_uint32(const std::string & str)
{
    std::stringstream ss;
    unsigned int res;
    ss.str(str);
    ss >> res;
    return res;
}


long long 
iegad::string::to_int64(const std::string & str)
{
    std::stringstream ss;
    long long res;
    ss.str(str);
    ss >> res;
    return res;
}


unsigned long long 
iegad::string::to_uint64(const std::string & str)
{
    std::stringstream ss;
    unsigned long long res;
    ss.str(str);
    ss >> res;
    return res;
}


float
iegad::string::to_float(const std::string & str)
{
    std::stringstream ss;
    float res;
    ss.str(str);
    ss >> res;
    return res;
}


double 
iegad::string::to_double(const std::string & str)
{
    std::stringstream ss;
    double res;
    ss.str(str);
    ss >> res;
    return res;
}


const std::string 
iegad::string::md5(const std::string & src)
{
    iegad::security::MD5 m(src);
    return m.hexdigest();
}

void 
iegad::string::sha1(const std::string & src, std::vector<unsigned int> & digest)
{
    unsigned int temp[5];
    digest.clear();
    boost::uuids::detail::sha1 sha;
    sha.process_bytes(src.c_str(), src.size());
    sha.get_digest(temp);
    for (int i = 0; i < 5; i++) {
	digest.push_back(temp[i]);
    }
}


const std::string 
iegad::string::base64_en(const std::string & src)
{
    using boost::archive::iterators::base64_from_binary;
    using boost::archive::iterators::transform_width;
    typedef base64_from_binary<transform_width<std::string::const_iterator, 6, 8>> base64_en_itor;
    std::stringstream result;
    std::copy(base64_en_itor(src.begin()), 
	base64_en_itor(src.end()), std::ostream_iterator<char>(result));
    size_t equal_count = (3 - src.length() % 3) % 3;
    for (size_t i = 0; i < equal_count; i++) {
	result.put('=');
    }
    return result.str();
}


const std::string 
iegad::string::base64_de(const std::string & src)
{
    using boost::archive::iterators::binary_from_base64;
    using boost::archive::iterators::transform_width;
    typedef transform_width<binary_from_base64<std::string::const_iterator>, 8, 6> base64_de_itor;
    std::stringstream result;
    std::copy(base64_de_itor(src.begin()),
	base64_de_itor(src.end() - 1), std::ostream_iterator<char>(result));
    return result.str();
}


const std::string 
iegad::string::guid()
{
    boost::uuids::random_generator rgen;
    boost::uuids::uuid u(rgen());
    return boost::uuids::to_string(u);
}


const std::string 
iegad::string::en_cust(const std::string & src, char key)
{
    std::string res;
    for (int i = 0, n = src.size(); i < n; i++) {
	res.push_back(src[i] ^ key);
    }
    return res;
}


const std::string 
iegad::string::de_cust(const std::string & src, char key)
{
    return en_cust(src, key);
}


const std::string 
iegad::string::format(const std::string & fmt, std::vector<std::string> & parms)
{
    /* =========================
    @ 这种算法灵活性太差
    int index = 0, len = 0, pos = 0, i = 0;
    std::string res;
    std::string flagstr;
    for (int i = 0, n = parms.size(); i < n; i++) {
	flagstr.clear();
	flagstr = "{" + iegad::string::to_str(i) + "}";
	index = fmt.find(flagstr);
	if (index == -1) {
	    return res;
	}
	res.append(fmt, pos, index - pos);
	res.append(parms[i]);
	pos = index + flagstr.size();
    }
    res.append(fmt, pos, fmt.size() - pos);
    return res;
    ========================= */

    std::string res = fmt, flag;
    for (int i = 0, n = parms.size(); i < n; i++) {
	flag = "{" + iegad::string::to_str(i) + "}";
	res = iegad::string::replace(res, flag, parms[i]);
    }
    return res;
}


int 
iegad::string::find_str(const std::string & src, const std::string & substr, int ntime /*= 1*/)
{
    int pos = 0, n = 0, index = 0;
    while (true) {
	pos = src.find(substr, index);
	if (pos == -1 || ++n == ntime) {
	    break;
	}
	index = pos + 1;
    }
    return pos;
}


bool 
iegad::string::start_with(const std::string & src, const std::string & substr)
{
    return src.find(substr, 0) == 0;
}


bool 
iegad::string::end_with(const std::string & src, const std::string & substr)
{
    std::string str = src.substr(src.size() - substr.size(), substr.size());
    return str == substr;
}


const std::string 
iegad::string::remove(const std::string & src, unsigned int pos, int n /*= -1*/)
{
    std::string res = src;
    int len = n < 0 ? src.size() : n;
    return res.erase(pos, len);
}


const std::string 
iegad::string::substr2(const std::string & src, unsigned int bgn, unsigned int end /*= -1*/)
{
    return std::string(src.begin() + bgn, src.begin() + (end == (unsigned int)(-1) ? src.size() : end + 1));
}


const std::string 
iegad::string::remove2(const std::string & src, unsigned int bgn, unsigned int end /*= -1*/)
{
    std::string res = src;
    end = end == -1 ? res.size() : end;
    res.erase(res.begin() + bgn, res.begin() + end + 1);
    return res;
}


/******************************************************/


