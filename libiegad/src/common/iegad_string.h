#ifndef __IEGAD_STRING__
#define __IEGAD_STRING__


// ============ 说明 ============
//
// @创建日期 : 2015-09-27
// @创建人 : iegad
//
// ============================
// @用途 : 对部分字符串算法操作的封装, 
//		此版本为v1.0, 以实现功能为主, 
//		可能存在一些性能上的问题.
//		关于性能上的优化, 会在下一版本中解决.
//
// @PS : std::string 为参数的 所有函数, 均不适合处理中文.
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================
//  --2015-10-03	    --iegad		   -- 添加 to_**(const std::string &) 这样的逆向转换函数
//  --2015-10-06	    --iegad		   -- 1, 添加md5, sha1 加密
//							   -- 2, 添加base64加密, 解密
//							   -- 3, 添加 guid 生成函数
//  --2015-10-09	    --iegad		   -- 添加 自定义加密 en_cust 函数, 解密 de_cust 函数 
//  --2015-10-13	    --iegad		   -- 1, 添加format函数, 用于格式化字符串
//							   -- 2, 修改to_str(double/float)的精度问题
//  --2015-10-17	    --iegad		   -- 修改, 完善format 的正确性和性能, 性能有所提升, 但是可能还存在提升的可能
//  --2015-10-20	    --iegad		   -- 1, 添加find_str 函数.
//							   -- 2, 添加start_with, end_with 函数.
//							   -- 3, 添加 remove 函数
//							   -- 4, 将各个 int pos 参数类型改为 unsigned int
//							   -- 5, 添加 substr2 函数
//  --2015-10-23	    --iegad		   -- 1, 修改 trim(const std::string &) 函数名 => rtrim(...);
//							   -- 2, 添加 新 trim(const std::string &)
//							   -- 3, 添加remove2(...) 函数
//  --2015-10-25	    --iegad		   -- 1, 添加 std::string & std::wstring 间的相互转换
//							   -- 2, UTF8 转换由原来的 STL 改为使用 boost实现, 因为, LINUX不支持 @include <codecvt>
//							   -- 3, 添加部分支持std::wstring 的字符串算法封装
//  --2015-11-10	    --iegad		   -- 修改 base64 编/解码 函数. 使之用于二进制数据
//  --2015-11-10	    --iegad		   -- 测试发现 新版的 base64 编/解码 算法有BUG, 还原回最初的版本(boost实现)
//  --2015-11-12	    --iegad		   -- base64 在编/解码时, 中间可能出现'\0', 无法避免, 添加 二进制 字符串 互相转换的函数 bin_tostr/ str_tobin


#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include "iegad_md5.h"
#include "iegad_define.h"


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
split_vct(const std::string & src, const std::string & chs);

// ============================
// @重载 : split_vct => std::wstring
// ============================
const std::vector<std::wstring>
split_vct(const std::wstring & src, const std::wstring & chs);


// ============================
// @用途 : 切割字符串, 将src由pos开始的n个字符 
//		切割成一个子字符串;
// @src : 源字符串
// @pos : 切割的开始位置
// @n : 指定切割的长度
// @返回值 : 切割后的子字符串
// ============================
const std::string
substr(const std::string & src, unsigned int pos, int n = -1);

// ============================
// @重载 : substr => std::wstring
// ============================
const std::wstring
substr(const std::wstring & src, unsigned int pos, int n = -1);


// ============================
// @用途 : 切割字符串, 将src以下标 bgn 为始, 下标 end为终
//		切割成一个子字符串;
// @src : 源字符串
// @bgn : 开始下标
// @end : 结束下标
// @返回值 : 切割后的子字符串
// @PS : 不同于substr的是 substr2 使用 下标方式来切割字符串.
//	    返回值包含 end 下标指表示的字符
// ============================
const std::string
substr2(const std::string & src, unsigned int bgn, unsigned int end = -1);

// ============================
// @重载 : substr2 => std::wstring
// ============================
const std::wstring
substr2(const std::wstring & src, unsigned int bgn, unsigned int end = -1);


// ============================
// @用途 : 去掉字符串src尾部的空白字符
// @src : 源字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
rtrim(const std::string & src);

// ============================
// @重载 : rtrim => std::wstring
// ============================
const std::wstring
rtrim(const std::wstring & src);


// ============================
// @用途 : 去掉字符串src中所有空白字符
// @src : 源字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
trim(const std::string & src);

// ============================
// @重载 : trim => std::wstring
// ============================
const std::wstring
trim(const std::wstring & src);


// ============================
// @用途 : 去掉字符串src中所有的指定的字符
// @src : 源字符串
// @chr : 指定去掉的字符
// @返回值 : 修改后的新字符串
// ============================
const std::string
trim(const std::string & src, char chr);

// ============================
// @重载 : trim => std::wstring
// ============================
const std::wstring
trim(const std::wstring & src, wchar_t chr);


// ============================
// @用途 : 去掉字符串src的首部空白字符
// @src : 源字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
ltrim(const std::string & src);

// ============================
// @重载 : ltrim => std::wstring
// ============================
const std::wstring
ltrim(const std::wstring & src);


// ============================
// @用途 : 将字符串src中的oldstr替换成newstr
// @src : 源字符串
// @oldstr : 需要被替换的字符串
// @newstr : 用来替换的新字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
replace(const std::string & src, const std::string & oldstr, const std::string & newstr);

// ============================
// @重载 : replace => std::wstring
// ============================
const std::wstring
replace(const std::wstring & src, const std::wstring & oldstr, const std::wstring & newstr);


// ============================
// @用途 : 在字符串src中找指定的字符chr, 并返回首次出现的下标
// @src : 源字符串
// @chr : 需要查找的字符串
// @返回值 : 若找到返回字符chr首次出现的下标, 否则返回-1
// ============================
int
fstchr(const std::string & src, char chr);

// ============================
// @重载 : fstchr => std::wstring
// ============================
int
fstchr(const std::wstring & src, wchar_t chr);


// ============================
// @用途 : 在字符串src中找指定的字符chr, 并返回最后一次出现的下标
// @src : 源字符串
// @chr : 需要查找的字符
// @返回值 : 若找到返回字符chr最后一次出现的下标, 否则返回-1
// ============================
int
lstchr(const std::string & src, char chr);

// ============================
// @重载 : lstchr => std::wstring
// ============================
int
lstchr(const std::wstring & src, wchar_t chr);


// ============================
// @用途 : 在字符串src中找指定的字符串substr第ntime次出现的位置, 并返回下标
// @src : 源字符串
// @substr : 需要查找的字符串
// @ntime : 出现的索引值, 默认为第一次出现的位置
// @返回值 : 若找到返回字符串substr 第ntime次 出现的下标, 否则返回-1
// ============================   
int
find_str(const std::string & src, const std::string & substr, int ntime = 1);

// ============================
// @重载 : find_str => std::wstring
// ============================
int
find_str(const std::wstring & src, const std::wstring & substr, int ntime = 1);


// ============================
// @用途 : 判断字符串 src 是否以字符串 substr 起头
// @src : 源字符串
// @substr : 需要判断的字符串
// @返回值 : 以substr 起头, 返回true, 否则返回 false
// ============================  
bool
start_with(const std::string & src, const std::string & substr);

// ============================
// @重载 : start_with => std::wstring
// ============================
bool
start_with(const std::wstring & src, const std::wstring & substr);


// ============================
// @用途 : 判断字符串 src 是否以字符串 substr 结尾
// @src : 源字符串
// @substr : 需要判断的字符串
// @返回值 : 以substr 结尾, 返回true, 否则返回 false
// ============================  
bool
end_with(const std::string & src, const std::string & substr);

// ============================
// @重载 : end_with => std::wstring
// ============================
bool
end_with(const std::wstring & src, const std::wstring & substr);


// ============================
// @用途 : 将src 字符串 从pos 处的 n 个字符删除
// @src : 源字符串
// @pos : 需要删除的开始下标
// @n : 需要删除的长度, 默认 -1, 表示从pos 开始直到结尾的部份都将删除
// @返回值 : 返回(删除)修改后的字符串
// ============================  
const std::string
remove(const std::string & src, unsigned int pos, int n = -1);

// ============================
// @重载 : remove => std::wstring
// ============================
const std::wstring
remove(const std::wstring & src, unsigned int pos, int n = -1);


// ============================
// @用途 : 将src 字符串 从 bgn下标 开始 到 end下标 的字符删除
// @src : 源字符串
// @bgn : 需要删除的开始下标
// @end : 需要删除结束下标, 默认 -1, 表示从src 的最后一个字符的下标
// @返回值 : 返回(删除)修改后的字符串
// ============================  
const std::string
remove2(const std::string & src, unsigned int bgn, unsigned int end = -1);

// ============================
// @重载 : remove2 => std::wstring
// ============================
const std::wstring
remove2(const std::wstring & src, unsigned int bgn, unsigned int end = -1);


// ============================
// @用途 : 将字符串src中所有的小写字母转成大写形式
// @src : 源字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
to_upr(const std::string & src);

// ============================
// @重载 : to_upr => std::wstring
// ============================
const std::wstring
to_upr(const std::wstring & src);


// ============================
// @用途 : 将字符串src中所有的大写字母转成小写形式
// @src : 源字符串
// @返回值 : 修改后的新字符串
// ============================
const std::string
to_lwr(const std::string & src);

// ============================
// @重载 : to_lwr => std::wstring
// ============================
const std::wstring
to_lwr(const std::wstring & src);


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
// @p : 有效数字, 默认为最合适的有效数字
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(float val, int p = FLOAT_DEFAULT_PRECISION);


// ============================
// @用途 : 将类型 double 转换成字符串形式
// @val : 需要转换的对象
// @p : 有效数字, 默认为最合适的有效数字
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(double val, int p = DOUBLE_DEFAULT_PRECISION);


// ============================
// @用途 : 将类型 long double 转换成字符串形式
// @val : 需要转换的对象
// @p : 有效数字, 默认为最合适的有效数字
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(long double val, int p = DECIMAL_DEFAULT_PRECISION);


// ============================
// @用途 : 将类型 bool 转换成字符串形式
// @val : 需要转换的对象
// @返回值 : 转换后的字符串
// ============================
const std::string
to_str(bool val);


// ============================
// @用途 : 将 std::string 类型 str 转换成 char
// @str : 需要转换的对象
// @返回值 : 字符串的第一个字符
// ============================
char
to_char(const std::string & str);


// ============================
// @用途 : 将 std::string 类型 str 转换成 short
// @str : 需要转换的对象
// @返回值 : str 为正确类型值时, 返回对应的值, 
//		    str 为非法值时, 返回值是随机的
// ============================
short
to_int16(const std::string & str);



// ============================
// @用途 : 将 std::string 类型 str 转换成 unsigned short
// @str : 需要转换的对象
// @返回值 : str 为正确类型值时, 返回对应的值, 
//		    str 为非法值时, 返回值是随机的
// ============================
unsigned short
to_uint16(const std::string & str);


// ============================
// @用途 : 将 std::string 类型 str 转换成 int
// @str : 需要转换的对象
// @返回值 : str 为正确类型值时, 返回对应的值, 
//		    str 为非法值时, 返回值是随机的
// ============================
int
to_int32(const std::string & str);


// ============================
// @用途 : 将 std::string 类型 str 转换成 unsigned int
// @str : 需要转换的对象
// @返回值 : str 为正确类型值时, 返回对应的值, 
//		    str 为非法值时, 返回值是随机的
// ============================
unsigned int
to_uint32(const std::string & str);


// ============================
// @用途 : 将 std::string 类型 str 转换成 long long
// @str : 需要转换的对象
// @返回值 : str 为正确类型值时, 返回对应的值, 
//		    str 为非法值时, 返回值是随机的
// ============================
long long
to_int64(const std::string & str);


// ============================
// @用途 : 将 std::string 类型 str 转换成 unsigned long long
// @str : 需要转换的对象
// @返回值 : str 为正确类型值时, 返回对应的值, 
//		    str 为非法值时, 返回值是随机的
// ============================
unsigned long long
to_uint64(const std::string & str);


// ============================
// @用途 : 将 std::string 类型 str 转换成 float
// @str : 需要转换的对象
// @返回值 : str 为正确类型值时, 返回对应的值, 
//		    str 为非法值时, 返回值是随机的
// ============================
float
to_float(const std::string & str);


// ============================
// @用途 : 将 std::string 类型 str 转换成 double
// @str : 需要转换的对象
// @返回值 : str 为正确类型值时, 返回对应的值, 
//		    str 为非法值时, 返回值是随机的
// ============================
double
to_double(const std::string & str);


// ============================
// @用途 : 将字符串 src 进行md5加密
// @src : 需要加密的字符串
// @返回值 : md5加密之后的字符串
// ============================
const std::string
md5(const std::string & src);


// ============================
// @用途 : 将字符串 src 进行sha1加密, 并将结果存放到 digest中
// @src : 需要加密的字符串
// @ser : 用来保存摘要的数组容器
// @返回值 : void
// ============================
void
sha1(const std::string & src, std::vector<unsigned int> & digest);


// ============================
// @用途 : 将 数据databuf 进行base64编码
// @databuf : 数据缓冲区
// @size : 数据长度
// @返回值 : 进行base64编码后的字符串
// ============================
const std::string
base64_en(const char * databuf, unsigned int size);


// ============================
// @用途 : 将字符串 src 进行base64解码
// @src : 需要解密的字符串
// @len : 输出参数, 输出数据长度
// @返回值 : 进行base64解码后的数据, 数据长度为 len
// @PS : 如果src 是未进行base64编码的字符串, 
//	    返回值是非预期的结果.
// ============================
const std::string
base64_de(const std::string & src);



// ============================
// @用途 : 将 数据databuf 编码 为字符串
// @databuf : 数据缓冲区
// @buff_size : 数据长度
// @返回值 : 编码后的字符串型式
// ============================
const std::string
bin_tostr(const char * buff, unsigned int buff_size);


// ============================
// @用途 : 将 字符串src 解码为 二进制数据
// @src : 需要解码的字符串
// @buff : 数据缓冲区, 输出参数
// @buff_size : 数据长度 输入/输出参数
// @返回值 : 解码成功返回 数据的首地址, 否则, 返回nullptr
// ============================
const char *
str_tobin(const std::string & src, char * buff, int & buff_size);


// ============================
// @用途 : 获取一个 36位 guid 字符串
// @返回值 : 一个 36 位, 小写, 中间还代有 短横杠的 guid字符串
// ============================
const std::string
guid();


// ============================
// @用途 : 将字符串 src 进行 自定义 加密
// @src : 需要加密的字符串
// @key : 密钥
// @返回值 : 加密后的字符串
// ============================
const std::string
en_cust(const std::string & src, char key);


// ============================
// @用途 : 将字符串 src 进行 自定义 解密
// @src : 需要解密的字符串
// @key : 密钥
// @返回值 : 解密后的字符串
// ============================
const std::string
de_cust(const std::string & src, char key);


// ============================
// @用途 : 字符串格式化
// @fmt : 格式串
// @parms : 参数
// @返回值 : 格式化后的字符串
// ============================
const std::string
format(const std::string & fmt, std::vector<std::string> & parms);


// ============================
// @用途 : std::string 转 std::wstring
// @src : std::string 字符串
// @charset : 当前字符集
// @返回值 : 转换后的std::wstring
// @PS : 不改变编码格式
// ============================
const std::wstring
str_towstr(const std::string & src, const std::string & charset = CHARSET_DEFAULT);


// ============================
// @用途 : std::wstring 转 std::string
// @src : std::wstring 字符串
// @charset : 当前字符集
// @返回值 : 转换后的std::string
// @PS : 不改变编码格式
// ============================
const std::string
wstr_tostr(const std::wstring & src, const std::string & charset = CHARSET_DEFAULT);


// ============================
// @用途 : 当前字符集 转成 UTF-8
// @srcstr : 需要转换的字符串
// @charset : 当前字符集
// @返回值 : 成功返回 UTF-8字符串, 否则 错误字符格式
// @PS : 转换后, 可能显示出乱码
// ============================
const std::string
to_utf8(const std::string & srcstr, const std::string & charset = CHARSET_DEFAULT);


// ============================
// @用途 : UTF-8 转成 当前字符集 
// @utf8str : UTF-8字符串
// @charset : 当前字符集
// @返回值 : 成功返回 当前字符集 字符串, 否则 错误字符格式
// ============================
const std::string
from_utf8(const std::string & utf8str, const std::string & charset = CHARSET_DEFAULT);


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
