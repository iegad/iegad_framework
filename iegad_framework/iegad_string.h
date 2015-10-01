#ifndef __IEGAD_STRING__
#define __IEGAD_STRING__


// ============ 说明 ============
//
// @创建日期 : 2015-09-27
// @创建人 : iegad
//
// ============================
// @用途 : 对部分字符串算法操作的封装
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================


#include <string>
#include <vector>
#include <cctype>


namespace iegad {
namespace string {


// ============================
// @用途 : 将src字符串根据chs
//		分组成为一个字符串向量
// @src : 源字符串
// @chs : 指定的字符, 可以指定多个字符
// @返回值 : 返回分组成功的字符串向量
// ============================
const std::vector<std::string>
split_vct(const std::string & src, const std::string chs);


// ============================
// @用途 : 切割字符串, 将src由pos开始的n个字符 
//		切割成一个子字符串;
// @src : 源字符串
// @pos : 切割的开始位置
// @n : 指定切割的长度
// @返回值 : 切割后的字符串
// ============================
const std::string
substr(const std::string & src, int pos, int n);


// ============================
// @用途 : 去掉字符串src尾部的空白字符
// @src : 源字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
trim(const std::string & src);


// ============================
// @用途 : 去掉字符串src中所有的指定的字符
// @src : 源字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
trim(const std::string & src, char chr);


// ============================
// @用途 : 去掉字符串src的首部空白字符
// @src : 源字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
ltrim(const std::string & src);


// ============================
// @用途 : 将字符串src中的oldstr替换成newstr
// @src : 源字符串
// @oldstr : 需要被替换的字符串
// @newstr : 用来替换的新字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
replace(const std::string & src, const std::string oldstr, const std::string newstr);


// ============================
// @用途 : 再字符串src中找指定的字符chr, 并返回首次出现的下标
// @src : 源字符串
// @chr : 需要查找的字符串.
// @返回值 : 若找到返回字符chr首次出现的下标, 否则返回-1
// ============================
int
fstchr(const std::string & src, char chr);


// ============================
// @用途 : 再字符串src中找指定的字符chr, 并返回最后一次出现的下标
// @src : 源字符串
// @chr : 需要查找的字符串
// @返回值 : 若找到返回字符chr最后一次出现的下标, 否则返回-1
// ============================
int
lstchr(const std::string & src, char chr);


// ============================
// @用途 : 将字符串src中所有的小写字母转成大写形式
// @src : 源字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
to_upr(const std::string & src);


// ============================
// @用途 : 将字符串src中所有的大写字母转成小写形式
// @src : 源字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
to_lwr(const std::string & src);


// ============================
// @用途 : 将类型 T 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// @PS : 该函数为模板函数, 大多to_str()函数, 都是以该模板函数实现.
// ============================
template <typename T>
const std::string
to_str(T val);


// ============================
// @用途 : 将类型 int 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(int val);


// ============================
// @用途 : 将类型 short 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(short val);


// ============================
// @用途 : 将类型 long 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(long val);


// ============================
// @用途 : 将类型 long long 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(long long val);


// ============================
// @用途 : 将类型 unsigned int 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(unsigned int val);


// ============================
// @用途 : 将类型 unsigned short 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(unsigned short val);


// ============================
// @用途 : 将类型 unsigned long 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(unsigned long val);


// ============================
// @用途 : 将类型 unsigned long long 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(unsigned long long val);


// ============================
// @用途 : 将类型 float 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(float val);


// ============================
// @用途 : 将类型 double 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(double val);


// ============================
// @用途 : 将类型 long double 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(long double val);


// ============================
// @用途 : 将类型 bool 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(bool val);


// ============================
// @用途 : 将std::wstring转换为UTF-8编码格式
// @val : 需要转换的unicode字符串
// @返回值 : 转换后的utf-8字符串 ( 将会显示乱码 )
// ============================
const std::string
unicode_to_utf8(const std::wstring & val);


// ============================
// @用途 : 将UTF-8格式字符串val转换为UNICODE编码格式的std::wstring
// @val : 需要转换的utf-8字符串
// @返回值 : 转换后的unicode字符串
// @PS : 记得在显示该std::wstring 时,
// 需要调用 std::locale::global(std::locale("Chinese-simplified")), 
// 否则无法正确显示unicode字符串
// ============================
const std::wstring
utf8_to_unicode(const std::string & val);


} //end namespace string


template <typename T>
const std::string iegad::string::to_str(T val)
{
    std::string res;
    std::stringstream strm;
    strm << val;
    strm >> res;
    return res;
}

} //end namespace string


#endif // __IEGAD_STRING__
