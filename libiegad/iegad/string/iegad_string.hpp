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
//  --2016-03-04	    --iegad		   -- 简化 to_upr & to_lwr
//  --2016-05-18	    --iegad		   --1, 去掉自定义加密/解密函数
//							   --2, 添加wstring 与 string 相互转换函数.
//							   --3, 添加format函数, 用来格式化字符串.
//  --2016-06-05    --iegad        -- 1, 将string操作改为hpp文件, 并更换为class实现
//                             --2, 将sha1改为非boost实现


#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include "sercurity/iegad_md5.hpp"
#include "iegad_define.in.h"
#include "sercurity/iegad_sha1.hpp"



namespace iegad {
class string_ex {
public:
    // ============================
    // @用途 : 将src字符串根据chs
    //		分组成为一个字符串向量
    // @src : 源字符串
    // @chs : 指定的字符, 可以指定多个字符
    // @返回值 : 返回分组成功的字符串向量
    // ============================
    static const std::vector<std::string>
    split_vct(const std::string & src, const std::string & chs)
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


    // ============================
    // @重载 : split_vct => std::wstring
    // ============================
    static const std::vector<std::wstring>
    split_vct(const std::wstring & src, const std::wstring & chs)
    {
        int pos = 0, len = chs.length(), n = 0, index = 0;
        std::vector<std::wstring> res;
        while (true) {
        index = src.find(chs, pos);
        if (index == -1) {
            res.push_back(std::wstring(src, pos, src.length() - pos));
            break;
        }
        n = index - n;
        res.push_back(std::wstring(src, pos, n));
        pos = index + len;
        n = pos;
        }
        return res;
    }


    // ============================
    // @用途 : 切割字符串, 将src由pos开始的n个字符
    //		切割成一个子字符串;
    // @src : 源字符串
    // @pos : 切割的开始位置
    // @n : 指定切割的长度
    // @返回值 : 切割后的子字符串
    // ============================
    static const std::string
    substr(const std::string & src, unsigned int pos, int n = -1)
    {
        int len = n < 0 ? src.size() - pos : n;
        std::string restr(src, pos, len);
        return restr;
    }


    // ============================
    // @重载 : substr => std::wstring
    // ============================
    static const std::wstring
    substr(const std::wstring & src, unsigned int pos, int n = -1)
    {
        int len = n < 0 ? src.size() - pos : n;
        std::wstring restr(src, pos, len);
        return restr;
    }


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
    static const std::string
    substr2(const std::string & src, unsigned int bgn, unsigned int end = -1)
    {
        return std::string(src.begin() + bgn,
                           src.begin() + (end == (unsigned int)(-1) ? src.size() : end + 1));
    }


    // ============================
    // @重载 : substr2 => std::wstring
    // ============================
    static const std::wstring
    substr2(const std::wstring & src, unsigned int bgn, unsigned int end = -1)
    {
        return std::wstring(src.begin() + bgn,
                            src.begin() + (end == (unsigned int)(-1) ? src.size() : end + 1));
    }


    // ============================
    // @用途 : 去掉字符串src尾部的空白字符
    // @src : 源字符串
    // @返回值 : 修改后的新字符串
    // ============================
    static const std::string
    rtrim(const std::string & src)
    {
        int n = src.length() - 1;
        while (std::isspace(src[n])) {
        n--;
        }
        std::string restr(src, 0, n + 1);
        return restr;
    }

    // ============================
    // @重载 : rtrim => std::wstring
    // ============================
    static const std::wstring
    rtrim(const std::wstring & src)
    {
        int n = src.length() - 1;
        while (::iswspace(src[n])) {
            n--;
        }
        std::wstring restr(src, 0, n + 1);
        return restr;
    }


    // ============================
    // @用途 : 去掉字符串src中所有空白字符
    // @src : 源字符串
    // @返回值 : 修改后的新字符串
    // ============================
    static const std::string
    trim(const std::string & src)
    {
        std::string restr;
        for (int i = 0, n = src.length(); i < n; i++) {
            if (::isspace(src[i])) {
                continue;
            }
            restr.push_back(src[i]);
        }
        return restr;
    }


    // ============================
    // @重载 : trim => std::wstring
    // ============================
    static const std::wstring
    trim(const std::wstring & src)
    {
        std::wstring restr;
        for (int i = 0, n = src.length(); i < n; i++) {
            if (::iswspace(src[i])) {
                continue;
            }
            restr.push_back(src[i]);
        }
        return restr;
    }


    // ============================
    // @用途 : 去掉字符串src中所有的指定的字符
    // @src : 源字符串
    // @chr : 指定去掉的字符
    // @返回值 : 修改后的新字符串
    // ============================
    static const std::string
    trim(const std::string & src, char chr)
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

    // ============================
    // @重载 : trim => std::wstring
    // ============================
    static const std::wstring
    trim(const std::wstring & src, wchar_t chr)
    {
        std::wstring restr;
        for (int i = 0, n = src.length(); i < n; i++) {
            if (src[i] == chr) {
                continue;
            }
            restr.push_back(src[i]);
        }
        return restr;
    }


    // ============================
    // @用途 : 去掉字符串src的首部空白字符
    // @src : 源字符串
    // @返回值 : 修改后的新字符串
    // ============================
    static const std::string
    ltrim(const std::string & src)
    {
        int rpos = 0;
        while (std::isspace(src[rpos])) {
            rpos++;
        }
        std::string restr(src, rpos, src.length() - rpos);
        return restr;
    }

    // ============================
    // @重载 : ltrim => std::wstring
    // ============================
    static const std::wstring
    ltrim(const std::wstring & src)
    {
        int rpos = 0;
        while (::iswspace(src[rpos])) {
            rpos++;
        }
        std::wstring restr(src, rpos, src.length() - rpos);
        return restr;
    }


    // ============================
    // @用途 : 将字符串src中的oldstr替换成newstr
    // @src : 源字符串
    // @oldstr : 需要被替换的字符串
    // @newstr : 用来替换的新字符串
    // @返回值 : 修改后的新字符串
    // ============================
    static const std::string
    replace(const std::string & src, const std::string & oldstr, const std::string & newstr)
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

    // ============================
    // @重载 : replace => std::wstring
    // ============================
    static const std::wstring
    replace(const std::wstring & src, const std::wstring & oldstr, const std::wstring & newstr)
    {
        int pos = 0, len = oldstr.length();
        std::wstring restr(src);
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


    // ============================
    // @用途 : 在字符串src中找指定的字符chr, 并返回首次出现的下标
    // @src : 源字符串
    // @chr : 需要查找的字符串
    // @返回值 : 若找到返回字符chr首次出现的下标, 否则返回-1
    // ============================
    static int
    first_ch(const std::string & src, char chr)
    {
        for (int i = 0, n = src.length(); i < n; i++) {
            if (src[i] == chr) {
                return i;
            }
        }
        return -1;
    }

    // ============================
    // @重载 : fstchr => std::wstring
    // ============================
    static int
    first_ch(const std::wstring & src, wchar_t chr)
    {
        for (int i = 0, n = src.length(); i < n; i++) {
        if (src[i] == chr) {
            return i;
        }
        }
        return -1;
    }


    // ============================
    // @用途 : 在字符串src中找指定的字符chr, 并返回最后一次出现的下标
    // @src : 源字符串
    // @chr : 需要查找的字符
    // @返回值 : 若找到返回字符chr最后一次出现的下标, 否则返回-1
    // ============================
    static int
    last_ch(const std::string & src, char chr)
    {
        for (int i = src.length() - 1; i >= 0; i--) {
            if (src[i] == chr) {
                return i;
            }
        }
        return -1;
    }


    // ============================
    // @重载 : lstchr => std::wstring
    // ============================
    static int
    last_ch(const std::wstring & src, wchar_t chr)
    {
        for (int i = src.length() - 1; i >= 0; i--) {
            if (src[i] == chr) {
                return i;
            }
        }
        return -1;
    }


    // ============================
    // @用途 : 在字符串src中找指定的字符串substr第ntime次出现的位置, 并返回下标
    // @src : 源字符串
    // @substr : 需要查找的字符串
    // @ntime : 出现的索引值, 默认为第一次出现的位置
    // @返回值 : 若找到返回字符串substr 第ntime次 出现的下标, 否则返回-1
    // ============================
    static int
    find_str(const std::string & src, const std::string & substr, int ntime = 1)
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

    // ============================
    // @重载 : find_str => std::wstring
    // ============================
    static int
    find_str(const std::wstring & src, const std::wstring & substr, int ntime = 1)
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


    // ============================
    // @用途 : 判断字符串 src 是否以字符串 substr 起头
    // @src : 源字符串
    // @substr : 需要判断的字符串
    // @返回值 : 以substr 起头, 返回true, 否则返回 false
    // ============================
    static bool
    start_with(const std::string & src, const std::string & substr)
    {
        return src.find(substr, 0) == 0;
    }

    // ============================
    // @重载 : start_with => std::wstring
    // ============================
    static bool
    start_with(const std::wstring & src, const std::wstring & substr)
    {
        return src.find(substr, 0) == 0;
    }


    // ============================
    // @用途 : 判断字符串 src 是否以字符串 substr 结尾
    // @src : 源字符串
    // @substr : 需要判断的字符串
    // @返回值 : 以substr 结尾, 返回true, 否则返回 false
    // ============================
    static bool
    end_with(const std::string & src, const std::string & substr)
    {
        std::string str = src.substr(src.size() - substr.size(), substr.size());
        return str == substr;
    }

    // ============================
    // @重载 : end_with => std::wstring
    // ============================
    static bool
    end_with(const std::wstring & src, const std::wstring & substr)
    {
        std::wstring str = src.substr(src.size() - substr.size(), substr.size());
        return str == substr;
    }


    // ============================
    // @用途 : 将src 字符串 从pos 处的 n 个字符删除
    // @src : 源字符串
    // @pos : 需要删除的开始下标
    // @n : 需要删除的长度, 默认 -1, 表示从pos 开始直到结尾的部份都将删除
    // @返回值 : 返回(删除)修改后的字符串
    // ============================
    static const std::string
    remove(const std::string & src, unsigned int pos, int n = -1)
    {
        std::string res = src;
        int len = n < 0 ? src.size() : n;
        return res.erase(pos, len);
    }

    // ============================
    // @重载 : remove => std::wstring
    // ============================
    static const std::wstring
    remove(const std::wstring & src, unsigned int pos, int n = -1)
    {
        std::wstring res = src;
        int len = n < 0 ? src.size() : n;
        return res.erase(pos, len);
    }


    // ============================
    // @用途 : 将src 字符串 从 bgn下标 开始 到 end下标 的字符删除
    // @src : 源字符串
    // @bgn : 需要删除的开始下标
    // @end : 需要删除结束下标, 默认 -1, 表示从src 的最后一个字符的下标
    // @返回值 : 返回(删除)修改后的字符串
    // ============================
    static const std::string
    remove2(const std::string & src, unsigned int bgn, unsigned int end = -1)
    {
        std::string res = src;
        end = static_cast<int>(end) == -1 ? res.size() : end;
        res.erase(res.begin() + bgn, res.begin() + end + 1);
        return res;
    }

    // ============================
    // @重载 : remove2 => std::wstring
    // ============================
    static const std::wstring
    remove2(const std::wstring & src, unsigned int bgn, unsigned int end = -1)
    {
        std::wstring res = src;
        end = static_cast<int>(end) == -1 ? res.size() : end;
        res.erase(res.begin() + bgn, res.begin() + end + 1);
        return res;
    }


    // ============================
    // @用途 : 将字符串src中所有的小写字母转成大写形式
    // @src : 源字符串
    // @返回值 : 修改后的新字符串
    // ============================
    static const std::string
    to_upr(const std::string & src)
    {
        std::string restr(src);
        std::transform(restr.begin(), restr.end(), restr.begin(), ::toupper);
        return restr;
    }

    // ============================
    // @重载 : to_upr => std::wstring
    // ============================
    static const std::wstring
    to_upr(const std::wstring & src)
    {
        std::wstring restr(src);
        std::transform(restr.begin(), restr.end(), restr.begin(), ::towupper);
        return restr;
    }


    // ============================
    // @用途 : 将字符串src中所有的大写字母转成小写形式
    // @src : 源字符串
    // @返回值 : 修改后的新字符串
    // ============================
    static const std::string
    to_lwr(const std::string & src)
    {
        std::string restr(src);
        std::transform(restr.begin(), restr.end(), restr.begin(), ::tolower);
        return restr;
    }

    // ============================
    // @重载 : to_lwr => std::wstring
    // ============================
    static const std::wstring
    to_lwr(const std::wstring & src)
    {
        std::wstring restr(src);
        std::transform(restr.begin(), restr.end(), restr.begin(), ::towlower);
        return restr;
    }


    // ============================
    // @用途 : 将字符串 src 进行md5加密
    // @src : 需要加密的字符串
    // @返回值 : md5加密之后的字符串
    // ============================
    static const std::string
    md5(const std::string & src)
    {
        iegad::security::MD5 m(src);
        return m.hexdigest();
    }


    // ============================
    // @用途 : 将字符串 src 进行sha1加密, 并将结果存放到 digest中
    // @src : 需要加密的字符串
    // @ser : 用来保存摘要的数组容器
    // @返回值 : void
    // ============================
    static const std::string
    sha1(const std::string & src)
    {
        iegad::security::SHA1 sha1;
        std::string res;
        return sha1.sha_go(src, &res) ? res : "";
    }


    // ============================
    // @用途 : 将 数据databuf 编码 为字符串
    // @databuf : 数据缓冲区
    // @buff_size : 数据长度
    // @返回值 : 编码后的字符串型式
    // ============================
    static const std::string
    bin_tostr(const char * buff, unsigned int buff_size)
    {
        std::string res(buff_size * 2, 0);
        uint8_t temp;

        for (size_t i = 0; i < buff_size; i++) {
        temp = buff[i];
            for (size_t j = 0; j < 2; j++) {
                uint8_t cCur = (temp & 0x0f);
                if (cCur < 10) {
                    cCur += '0';
                }
                else {
                    cCur += ('A' - 10);
                }
                res[2 * i + 1 - j] = cCur;
                temp >>= 4;
            }
        }
        return res;
    }


    // ============================
    // @用途 : 将 字符串src 解码为 二进制数据
    // @src : 需要解码的字符串
    // @buff : 数据缓冲区, 输出参数
    // @buff_size : 数据长度 输入/输出参数
    // @返回值 : 解码成功返回 数据的首地址, 否则, 返回nullptr
    // ============================
    static const char *
    str_tobin(const std::string & src, char * buff, int & buff_size)
    {
        if (src.size() % 2 != 0 ||
            buff == nullptr ||
            static_cast<size_t>(buff_size) < src.size() / 2) {
            return nullptr;
        }

        buff_size = src.size() / 2;
        for (int i = 0; i < buff_size; i++) {
            uint8_t cTemp = 0;
            for (size_t j = 0; j < 2; j++) {
                char cCur = src[2 * i + j];
                if (cCur >= '0' && cCur <= '9') {
                    cTemp = (cTemp << 4) + (cCur - '0');
                }
                /*else if (cCur >= 'a' && cCur <= 'f') {
                cTemp = (cTemp << 4) + (cCur - 'a' + 10);
                }*/
                else if (cCur >= 'A' && cCur <= 'F') {
                    cTemp = (cTemp << 4) + (cCur - 'A' + 10);
                }
                else {
                    return nullptr;
                }
            } // for (size_t j = 0; j < 2; j++);
            buff[i] = cTemp;
        } // for (size_t i = 0; i < buff_size; i++);

        return buff;
    }


    // ============================
    // @用途 : 获取一个 36位 guid 字符串
    // @返回值 : 一个 36 位, 小写, 中间还代有 短横杠的 guid字符串
    // ============================
    static const std::string
    guid();


    // ============================
    // @用途 : 将std::wstring 转换成std::string
    // @wstr : 需要std::wstring 宽字符串
    // @返回值 : 转换后的std::string类型字符串
    // ============================
    static const std::string
    wstr_to_str(const std::wstring & wstr)
    {
        const wchar_t* _Source = wstr.c_str();
        size_t _Dsize = 2 * wstr.size() + 1;
        char *_Dest = new char[_Dsize];
        memset(_Dest, 0, _Dsize);
        wcstombs(_Dest, _Source, _Dsize);
        std::string res = _Dest;
        delete[]_Dest;
        return res;
    }


    // ============================
    // @用途 : 将std::string 转换成std::wstring
    // @str : 需要std::string 字符串
    // @返回值 : 转换后的std::wstring类型字符串
    // ============================
    static const std::wstring
    str_to_wstr(const std::string & str)
    {
        const char* _Source = str.c_str();
        size_t _Dsize = str.size() + 1;
        wchar_t *_Dest = new wchar_t[_Dsize];
        wmemset(_Dest, 0, _Dsize);
        mbstowcs(_Dest, _Source, _Dsize);
        std::wstring res = _Dest;
        delete[]_Dest;
        return res;
    }


    // ============================
    // @用途 : 将std::string 转换成std::wstring
    // @fmt : 格式化字符串
    // @params : 参数列表.
    // @返回值 : 格式化后的字符串
    // @PS : 使用C#的Format格式 {0}, {1}, {2} ...
    // ============================
    static const std::string
    format(const std::string & fmt, const std::initializer_list<std::string> & params)
    {
        std::string temp, res = fmt;
        int i = 0;
        for (auto itor = params.begin(); itor != params.end(); itor++, i++) {
            temp = std::string("{") + std::to_string(i) + "}";
            res = iegad::string_ex::replace(res, temp, *itor);
        }
        return res;
    }

    // ============================
    // @重载 : format => std::wstring
    // ============================
    static const std::wstring
    format(const std::wstring & fmt, const std::initializer_list<std::wstring> & params)
    {
        std::wstring temp, res = fmt;
        int i = 0;
        for (auto itor = params.begin(); itor != params.end(); itor++, i++) {
            temp = std::wstring(L"{") + std::to_wstring(i) + L"}";
            res = iegad::string_ex::replace(res, temp, *itor);
        }
        return res;
    }



}; // class string_ex;
} // namespace iegad;


#endif // __IEGAD_STRING__
