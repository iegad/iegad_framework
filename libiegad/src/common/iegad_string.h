#ifndef __IEGAD_STRING__
#define __IEGAD_STRING__


// ============ ˵�� ============
//
// @�������� : 2015-09-27
// @������ : iegad
//
// ============================
// @��; : �Բ����ַ����㷨�����ķ�װ, 
//		�˰汾Ϊv1.0, ��ʵ�ֹ���Ϊ��, 
//		���ܴ���һЩ�����ϵ�����.
//		���������ϵ��Ż�, ������һ�汾�н��.
//
// @PS : std::string Ϊ������ ���к���, �����ʺϴ�������.
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================
//  --2015-10-03	    --iegad		   -- ��� to_**(const std::string &) ����������ת������
//  --2015-10-06	    --iegad		   -- 1, ���md5, sha1 ����
//							   -- 2, ���base64����, ����
//							   -- 3, ��� guid ���ɺ���
//  --2015-10-09	    --iegad		   -- ��� �Զ������ en_cust ����, ���� de_cust ���� 
//  --2015-10-13	    --iegad		   -- 1, ���format����, ���ڸ�ʽ���ַ���
//							   -- 2, �޸�to_str(double/float)�ľ�������
//  --2015-10-17	    --iegad		   -- �޸�, ����format ����ȷ�Ժ�����, ������������, ���ǿ��ܻ����������Ŀ���
//  --2015-10-20	    --iegad		   -- 1, ���find_str ����.
//							   -- 2, ���start_with, end_with ����.
//							   -- 3, ��� remove ����
//							   -- 4, ������ int pos �������͸�Ϊ unsigned int
//							   -- 5, ��� substr2 ����
//  --2015-10-23	    --iegad		   -- 1, �޸� trim(const std::string &) ������ => rtrim(...);
//							   -- 2, ��� �� trim(const std::string &)
//							   -- 3, ���remove2(...) ����
//  --2015-10-25	    --iegad		   -- 1, ��� std::string & std::wstring ����໥ת��
//							   -- 2, UTF8 ת����ԭ���� STL ��Ϊʹ�� boostʵ��, ��Ϊ, LINUX��֧�� @include <codecvt>
//							   -- 3, ��Ӳ���֧��std::wstring ���ַ����㷨��װ
//  --2015-11-10	    --iegad		   -- �޸� base64 ��/���� ����. ʹ֮���ڶ���������
//  --2015-11-10	    --iegad		   -- ���Է��� �°�� base64 ��/���� �㷨��BUG, ��ԭ������İ汾(boostʵ��)
//  --2015-11-12	    --iegad		   -- base64 �ڱ�/����ʱ, �м���ܳ���'\0', �޷�����, ��� ������ �ַ��� ����ת���ĺ��� bin_tostr/ str_tobin


#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include "iegad_md5.h"
#include "iegad_define.h"


namespace iegad {
namespace string {


// ============================
// @��; : ��src�ַ�������chs
//		�����Ϊһ���ַ�������
// @src : Դ�ַ���
// @chs : ָ�����ַ�, ����ָ������ַ�
// @����ֵ : ���ط���ɹ����ַ�������
// ============================
const std::vector<std::string>
split_vct(const std::string & src, const std::string & chs);

// ============================
// @���� : split_vct => std::wstring
// ============================
const std::vector<std::wstring>
split_vct(const std::wstring & src, const std::wstring & chs);


// ============================
// @��; : �и��ַ���, ��src��pos��ʼ��n���ַ� 
//		�и��һ�����ַ���;
// @src : Դ�ַ���
// @pos : �и�Ŀ�ʼλ��
// @n : ָ���и�ĳ���
// @����ֵ : �и������ַ���
// ============================
const std::string
substr(const std::string & src, unsigned int pos, int n = -1);

// ============================
// @���� : substr => std::wstring
// ============================
const std::wstring
substr(const std::wstring & src, unsigned int pos, int n = -1);


// ============================
// @��; : �и��ַ���, ��src���±� bgn Ϊʼ, �±� endΪ��
//		�и��һ�����ַ���;
// @src : Դ�ַ���
// @bgn : ��ʼ�±�
// @end : �����±�
// @����ֵ : �и������ַ���
// @PS : ��ͬ��substr���� substr2 ʹ�� �±귽ʽ���и��ַ���.
//	    ����ֵ���� end �±�ָ��ʾ���ַ�
// ============================
const std::string
substr2(const std::string & src, unsigned int bgn, unsigned int end = -1);

// ============================
// @���� : substr2 => std::wstring
// ============================
const std::wstring
substr2(const std::wstring & src, unsigned int bgn, unsigned int end = -1);


// ============================
// @��; : ȥ���ַ���srcβ���Ŀհ��ַ�
// @src : Դ�ַ���
// @����ֵ : �޸ĺ�����ַ���
// ============================
const std::string
rtrim(const std::string & src);

// ============================
// @���� : rtrim => std::wstring
// ============================
const std::wstring
rtrim(const std::wstring & src);


// ============================
// @��; : ȥ���ַ���src�����пհ��ַ�
// @src : Դ�ַ���
// @����ֵ : �޸ĺ�����ַ���
// ============================
const std::string
trim(const std::string & src);

// ============================
// @���� : trim => std::wstring
// ============================
const std::wstring
trim(const std::wstring & src);


// ============================
// @��; : ȥ���ַ���src�����е�ָ�����ַ�
// @src : Դ�ַ���
// @chr : ָ��ȥ�����ַ�
// @����ֵ : �޸ĺ�����ַ���
// ============================
const std::string
trim(const std::string & src, char chr);

// ============================
// @���� : trim => std::wstring
// ============================
const std::wstring
trim(const std::wstring & src, wchar_t chr);


// ============================
// @��; : ȥ���ַ���src���ײ��հ��ַ�
// @src : Դ�ַ���
// @����ֵ : �޸ĺ�����ַ���
// ============================
const std::string
ltrim(const std::string & src);

// ============================
// @���� : ltrim => std::wstring
// ============================
const std::wstring
ltrim(const std::wstring & src);


// ============================
// @��; : ���ַ���src�е�oldstr�滻��newstr
// @src : Դ�ַ���
// @oldstr : ��Ҫ���滻���ַ���
// @newstr : �����滻�����ַ���
// @����ֵ : �޸ĺ�����ַ���
// ============================
const std::string
replace(const std::string & src, const std::string & oldstr, const std::string & newstr);

// ============================
// @���� : replace => std::wstring
// ============================
const std::wstring
replace(const std::wstring & src, const std::wstring & oldstr, const std::wstring & newstr);


// ============================
// @��; : ���ַ���src����ָ�����ַ�chr, �������״γ��ֵ��±�
// @src : Դ�ַ���
// @chr : ��Ҫ���ҵ��ַ���
// @����ֵ : ���ҵ������ַ�chr�״γ��ֵ��±�, ���򷵻�-1
// ============================
int
fstchr(const std::string & src, char chr);

// ============================
// @���� : fstchr => std::wstring
// ============================
int
fstchr(const std::wstring & src, wchar_t chr);


// ============================
// @��; : ���ַ���src����ָ�����ַ�chr, ���������һ�γ��ֵ��±�
// @src : Դ�ַ���
// @chr : ��Ҫ���ҵ��ַ�
// @����ֵ : ���ҵ������ַ�chr���һ�γ��ֵ��±�, ���򷵻�-1
// ============================
int
lstchr(const std::string & src, char chr);

// ============================
// @���� : lstchr => std::wstring
// ============================
int
lstchr(const std::wstring & src, wchar_t chr);


// ============================
// @��; : ���ַ���src����ָ�����ַ���substr��ntime�γ��ֵ�λ��, �������±�
// @src : Դ�ַ���
// @substr : ��Ҫ���ҵ��ַ���
// @ntime : ���ֵ�����ֵ, Ĭ��Ϊ��һ�γ��ֵ�λ��
// @����ֵ : ���ҵ������ַ���substr ��ntime�� ���ֵ��±�, ���򷵻�-1
// ============================   
int
find_str(const std::string & src, const std::string & substr, int ntime = 1);

// ============================
// @���� : find_str => std::wstring
// ============================
int
find_str(const std::wstring & src, const std::wstring & substr, int ntime = 1);


// ============================
// @��; : �ж��ַ��� src �Ƿ����ַ��� substr ��ͷ
// @src : Դ�ַ���
// @substr : ��Ҫ�жϵ��ַ���
// @����ֵ : ��substr ��ͷ, ����true, ���򷵻� false
// ============================  
bool
start_with(const std::string & src, const std::string & substr);

// ============================
// @���� : start_with => std::wstring
// ============================
bool
start_with(const std::wstring & src, const std::wstring & substr);


// ============================
// @��; : �ж��ַ��� src �Ƿ����ַ��� substr ��β
// @src : Դ�ַ���
// @substr : ��Ҫ�жϵ��ַ���
// @����ֵ : ��substr ��β, ����true, ���򷵻� false
// ============================  
bool
end_with(const std::string & src, const std::string & substr);

// ============================
// @���� : end_with => std::wstring
// ============================
bool
end_with(const std::wstring & src, const std::wstring & substr);


// ============================
// @��; : ��src �ַ��� ��pos ���� n ���ַ�ɾ��
// @src : Դ�ַ���
// @pos : ��Ҫɾ���Ŀ�ʼ�±�
// @n : ��Ҫɾ���ĳ���, Ĭ�� -1, ��ʾ��pos ��ʼֱ����β�Ĳ��ݶ���ɾ��
// @����ֵ : ����(ɾ��)�޸ĺ���ַ���
// ============================  
const std::string
remove(const std::string & src, unsigned int pos, int n = -1);

// ============================
// @���� : remove => std::wstring
// ============================
const std::wstring
remove(const std::wstring & src, unsigned int pos, int n = -1);


// ============================
// @��; : ��src �ַ��� �� bgn�±� ��ʼ �� end�±� ���ַ�ɾ��
// @src : Դ�ַ���
// @bgn : ��Ҫɾ���Ŀ�ʼ�±�
// @end : ��Ҫɾ�������±�, Ĭ�� -1, ��ʾ��src �����һ���ַ����±�
// @����ֵ : ����(ɾ��)�޸ĺ���ַ���
// ============================  
const std::string
remove2(const std::string & src, unsigned int bgn, unsigned int end = -1);

// ============================
// @���� : remove2 => std::wstring
// ============================
const std::wstring
remove2(const std::wstring & src, unsigned int bgn, unsigned int end = -1);


// ============================
// @��; : ���ַ���src�����е�Сд��ĸת�ɴ�д��ʽ
// @src : Դ�ַ���
// @����ֵ : �޸ĺ�����ַ���
// ============================
const std::string
to_upr(const std::string & src);

// ============================
// @���� : to_upr => std::wstring
// ============================
const std::wstring
to_upr(const std::wstring & src);


// ============================
// @��; : ���ַ���src�����еĴ�д��ĸת��Сд��ʽ
// @src : Դ�ַ���
// @����ֵ : �޸ĺ�����ַ���
// ============================
const std::string
to_lwr(const std::string & src);

// ============================
// @���� : to_lwr => std::wstring
// ============================
const std::wstring
to_lwr(const std::wstring & src);


// ============================
// @��; : ������ T ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @����ֵ : ת������ַ���
// @PS : �ú���Ϊģ�庯��, ���to_str()����, �����Ը�ģ�庯��ʵ��.
// ============================
template <typename T>
const std::string
to_str(T val);


// ============================
// @��; : ������ int ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(int val);


// ============================
// @��; : ������ short ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(short val);


// ============================
// @��; : ������ long ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(long val);


// ============================
// @��; : ������ long long ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(long long val);


// ============================
// @��; : ������ unsigned int ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(unsigned int val);


// ============================
// @��; : ������ unsigned short ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(unsigned short val);


// ============================
// @��; : ������ unsigned long ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(unsigned long val);


// ============================
// @��; : ������ unsigned long long ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(unsigned long long val);


// ============================
// @��; : ������ float ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @p : ��Ч����, Ĭ��Ϊ����ʵ���Ч����
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(float val, int p = FLOAT_DEFAULT_PRECISION);


// ============================
// @��; : ������ double ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @p : ��Ч����, Ĭ��Ϊ����ʵ���Ч����
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(double val, int p = DOUBLE_DEFAULT_PRECISION);


// ============================
// @��; : ������ long double ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @p : ��Ч����, Ĭ��Ϊ����ʵ���Ч����
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(long double val, int p = DECIMAL_DEFAULT_PRECISION);


// ============================
// @��; : ������ bool ת�����ַ�����ʽ
// @val : ��Ҫת���Ķ���
// @����ֵ : ת������ַ���
// ============================
const std::string
to_str(bool val);


// ============================
// @��; : �� std::string ���� str ת���� char
// @str : ��Ҫת���Ķ���
// @����ֵ : �ַ����ĵ�һ���ַ�
// ============================
char
to_char(const std::string & str);


// ============================
// @��; : �� std::string ���� str ת���� short
// @str : ��Ҫת���Ķ���
// @����ֵ : str Ϊ��ȷ����ֵʱ, ���ض�Ӧ��ֵ, 
//		    str Ϊ�Ƿ�ֵʱ, ����ֵ�������
// ============================
short
to_int16(const std::string & str);



// ============================
// @��; : �� std::string ���� str ת���� unsigned short
// @str : ��Ҫת���Ķ���
// @����ֵ : str Ϊ��ȷ����ֵʱ, ���ض�Ӧ��ֵ, 
//		    str Ϊ�Ƿ�ֵʱ, ����ֵ�������
// ============================
unsigned short
to_uint16(const std::string & str);


// ============================
// @��; : �� std::string ���� str ת���� int
// @str : ��Ҫת���Ķ���
// @����ֵ : str Ϊ��ȷ����ֵʱ, ���ض�Ӧ��ֵ, 
//		    str Ϊ�Ƿ�ֵʱ, ����ֵ�������
// ============================
int
to_int32(const std::string & str);


// ============================
// @��; : �� std::string ���� str ת���� unsigned int
// @str : ��Ҫת���Ķ���
// @����ֵ : str Ϊ��ȷ����ֵʱ, ���ض�Ӧ��ֵ, 
//		    str Ϊ�Ƿ�ֵʱ, ����ֵ�������
// ============================
unsigned int
to_uint32(const std::string & str);


// ============================
// @��; : �� std::string ���� str ת���� long long
// @str : ��Ҫת���Ķ���
// @����ֵ : str Ϊ��ȷ����ֵʱ, ���ض�Ӧ��ֵ, 
//		    str Ϊ�Ƿ�ֵʱ, ����ֵ�������
// ============================
long long
to_int64(const std::string & str);


// ============================
// @��; : �� std::string ���� str ת���� unsigned long long
// @str : ��Ҫת���Ķ���
// @����ֵ : str Ϊ��ȷ����ֵʱ, ���ض�Ӧ��ֵ, 
//		    str Ϊ�Ƿ�ֵʱ, ����ֵ�������
// ============================
unsigned long long
to_uint64(const std::string & str);


// ============================
// @��; : �� std::string ���� str ת���� float
// @str : ��Ҫת���Ķ���
// @����ֵ : str Ϊ��ȷ����ֵʱ, ���ض�Ӧ��ֵ, 
//		    str Ϊ�Ƿ�ֵʱ, ����ֵ�������
// ============================
float
to_float(const std::string & str);


// ============================
// @��; : �� std::string ���� str ת���� double
// @str : ��Ҫת���Ķ���
// @����ֵ : str Ϊ��ȷ����ֵʱ, ���ض�Ӧ��ֵ, 
//		    str Ϊ�Ƿ�ֵʱ, ����ֵ�������
// ============================
double
to_double(const std::string & str);


// ============================
// @��; : ���ַ��� src ����md5����
// @src : ��Ҫ���ܵ��ַ���
// @����ֵ : md5����֮����ַ���
// ============================
const std::string
md5(const std::string & src);


// ============================
// @��; : ���ַ��� src ����sha1����, ���������ŵ� digest��
// @src : ��Ҫ���ܵ��ַ���
// @ser : ��������ժҪ����������
// @����ֵ : void
// ============================
void
sha1(const std::string & src, std::vector<unsigned int> & digest);


// ============================
// @��; : �� ����databuf ����base64����
// @databuf : ���ݻ�����
// @size : ���ݳ���
// @����ֵ : ����base64�������ַ���
// ============================
const std::string
base64_en(const char * databuf, unsigned int size);


// ============================
// @��; : ���ַ��� src ����base64����
// @src : ��Ҫ���ܵ��ַ���
// @len : �������, ������ݳ���
// @����ֵ : ����base64����������, ���ݳ���Ϊ len
// @PS : ���src ��δ����base64������ַ���, 
//	    ����ֵ�Ƿ�Ԥ�ڵĽ��.
// ============================
const std::string
base64_de(const std::string & src);



// ============================
// @��; : �� ����databuf ���� Ϊ�ַ���
// @databuf : ���ݻ�����
// @buff_size : ���ݳ���
// @����ֵ : �������ַ�����ʽ
// ============================
const std::string
bin_tostr(const char * buff, unsigned int buff_size);


// ============================
// @��; : �� �ַ���src ����Ϊ ����������
// @src : ��Ҫ������ַ���
// @buff : ���ݻ�����, �������
// @buff_size : ���ݳ��� ����/�������
// @����ֵ : ����ɹ����� ���ݵ��׵�ַ, ����, ����nullptr
// ============================
const char *
str_tobin(const std::string & src, char * buff, int & buff_size);


// ============================
// @��; : ��ȡһ�� 36λ guid �ַ���
// @����ֵ : һ�� 36 λ, Сд, �м仹���� �̺�ܵ� guid�ַ���
// ============================
const std::string
guid();


// ============================
// @��; : ���ַ��� src ���� �Զ��� ����
// @src : ��Ҫ���ܵ��ַ���
// @key : ��Կ
// @����ֵ : ���ܺ���ַ���
// ============================
const std::string
en_cust(const std::string & src, char key);


// ============================
// @��; : ���ַ��� src ���� �Զ��� ����
// @src : ��Ҫ���ܵ��ַ���
// @key : ��Կ
// @����ֵ : ���ܺ���ַ���
// ============================
const std::string
de_cust(const std::string & src, char key);


// ============================
// @��; : �ַ�����ʽ��
// @fmt : ��ʽ��
// @parms : ����
// @����ֵ : ��ʽ������ַ���
// ============================
const std::string
format(const std::string & fmt, std::vector<std::string> & parms);


// ============================
// @��; : std::string ת std::wstring
// @src : std::string �ַ���
// @charset : ��ǰ�ַ���
// @����ֵ : ת�����std::wstring
// @PS : ���ı�����ʽ
// ============================
const std::wstring
str_towstr(const std::string & src, const std::string & charset = CHARSET_DEFAULT);


// ============================
// @��; : std::wstring ת std::string
// @src : std::wstring �ַ���
// @charset : ��ǰ�ַ���
// @����ֵ : ת�����std::string
// @PS : ���ı�����ʽ
// ============================
const std::string
wstr_tostr(const std::wstring & src, const std::string & charset = CHARSET_DEFAULT);


// ============================
// @��; : ��ǰ�ַ��� ת�� UTF-8
// @srcstr : ��Ҫת�����ַ���
// @charset : ��ǰ�ַ���
// @����ֵ : �ɹ����� UTF-8�ַ���, ���� �����ַ���ʽ
// @PS : ת����, ������ʾ������
// ============================
const std::string
to_utf8(const std::string & srcstr, const std::string & charset = CHARSET_DEFAULT);


// ============================
// @��; : UTF-8 ת�� ��ǰ�ַ��� 
// @utf8str : UTF-8�ַ���
// @charset : ��ǰ�ַ���
// @����ֵ : �ɹ����� ��ǰ�ַ��� �ַ���, ���� �����ַ���ʽ
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
