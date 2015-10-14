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
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================
//  --2015-10-03	    --iegad		    添加 to_**(const std::string &) 这样的逆向转换函数
//  --2015-10-06	    --iegad		    1, 添加md5, sha1 加密
//							    2, 添加base64加密, 解密
//							    3, 添加 guid 生成函数
//  --2015-10-09	    --iegad		    添加 自定义加密 en_cust 函数, 解密 de_cust 函数 
//  --2015-10-13	    --iegad		    1, 添加format函数, 用于格式化字符串
//							    2, 修改to_str(double/float)的精度问题


#include <string>
#include <vector>
#include <cctype>
#include "security/iegad_md5.h"
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
// @用途 : 切割字符串, 将src由pos开始的n个字符 
//		切割成一个子字符串;
// @src : 源字符串
// @pos : 切割的开始位置
// @n : 指定切割的长度
// @返回值 : 切割后的子字符串
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
replace(const std::string & src, const std::string & oldstr, const std::string & newstr);


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
// @用途 : 将字符串 src 进行base64加密
// @src : 需要加密的字符串
// @返回值 : 进行base64加密后的字符串
// ============================
const std::string
base64_en(const std::string & src);


// ============================
// @用途 : 将字符串 src 进行base64解密
// @src : 需要解密的字符串
// @返回值 : 进行base64解密后的字符串
// @PS : 如果src 是未进行base64加密的字符串, 
//	    返回值是非预期的结果.
// ============================
const std::string
base64_de(const std::string & src);


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
