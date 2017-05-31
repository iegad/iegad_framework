// ==========================================
// @title : string 第一轮测试
// @by : iegad, 2016-06-17
// ==========================================


#include <gtest/gtest.h>
#include "string/iegad_string.hpp"
#include "sercurity/iegad_security.hpp"
#include "tools/iegad_log.hpp"

using namespace iegad;



// 1: ============= split =============

TEST(stringTest, split_1)
{// 单个分隔符
    std::string src = "00,01,02,03,04";
    std::vector<std::string> res = string::split_vct(src, ",");
    EXPECT_EQ(5u, res.size());
    EXPECT_EQ("00", res[0]);
    EXPECT_EQ("01", res[1]);
    EXPECT_EQ("02", res[2]);
    EXPECT_EQ("03", res[3]);
    EXPECT_EQ("04", res[4]);

    std::wstring wsrc = L"00,01,02,03,04";
    std::vector<std::wstring> wres = string::split_vct(wsrc, L",");
    EXPECT_EQ(5u, wres.size());
    EXPECT_EQ(L"00", wres[0]);
    EXPECT_EQ(L"01", wres[1]);
    EXPECT_EQ(L"02", wres[2]);
    EXPECT_EQ(L"03", wres[3]);
    EXPECT_EQ(L"04", wres[4]);
}


TEST(stringTest, split_2)
{// 结尾出现分隔符
    std::string src = "00,01,02,03,04,";
    std::vector<std::string> res = string::split_vct(src, ",");
    EXPECT_EQ(6u, res.size());
    EXPECT_EQ("00", res[0]);
    EXPECT_EQ("01", res[1]);
    EXPECT_EQ("02", res[2]);
    EXPECT_EQ("03", res[3]);
    EXPECT_EQ("04", res[4]);
    EXPECT_EQ("", res[5]);
    EXPECT_TRUE(res[5].empty());

    std::wstring wsrc = L"00,01,02,03,04,";
    std::vector<std::wstring> wres = string::split_vct(wsrc, L",");
    EXPECT_EQ(6u, wres.size());
    EXPECT_EQ(L"00", wres[0]);
    EXPECT_EQ(L"01", wres[1]);
    EXPECT_EQ(L"02", wres[2]);
    EXPECT_EQ(L"03", wres[3]);
    EXPECT_EQ(L"04", wres[4]);
    EXPECT_EQ(L"", wres[5]);
    EXPECT_TRUE(wres[5].empty());
}


TEST(stringTest, split_3)
{// 中间出现分隔符
    std::string src = "00,01,,02,03,04";
    std::vector<std::string> res = string::split_vct(src, ",");
    EXPECT_EQ(6u, res.size());
    EXPECT_EQ("00", res[0]);
    EXPECT_EQ("01", res[1]);
    EXPECT_EQ("", res[2]);
    EXPECT_EQ("02", res[3]);
    EXPECT_EQ("03", res[4]);
    EXPECT_EQ("04", res[5]);
    EXPECT_TRUE(res[2].empty());

    std::wstring wsrc = L"00,01,,02,03,04";
    std::vector<std::wstring> wres = string::split_vct(wsrc, L",");
    EXPECT_EQ(6u, wres.size());
    EXPECT_EQ(L"00", wres[0]);
    EXPECT_EQ(L"01", wres[1]);
    EXPECT_EQ(L"", wres[2]);
    EXPECT_EQ(L"02", wres[3]);
    EXPECT_EQ(L"03", wres[4]);
    EXPECT_EQ(L"04", wres[5]);
    EXPECT_TRUE(wres[2].empty());
}


TEST(stringTest, split_4)
{// 开始处出现分隔符
    std::string src = ",00,01,02,03,04";
    std::vector<std::string> res = string::split_vct(src, ",");
    EXPECT_EQ(6u, res.size());
    EXPECT_EQ("", res[0]);
    EXPECT_EQ("00", res[1]);
    EXPECT_EQ("01", res[2]);
    EXPECT_EQ("02", res[3]);
    EXPECT_EQ("03", res[4]);
    EXPECT_EQ("04", res[5]);
    EXPECT_TRUE(res[0].empty());

    std::wstring wsrc = L",00,01,02,03,04";
    std::vector<std::wstring> wres = string::split_vct(wsrc, L",");
    EXPECT_EQ(6u, wres.size());
    EXPECT_EQ(L"", wres[0]);
    EXPECT_EQ(L"00", wres[1]);
    EXPECT_EQ(L"01", wres[2]);
    EXPECT_EQ(L"02", wres[3]);
    EXPECT_EQ(L"03", wres[4]);
    EXPECT_EQ(L"04", wres[5]);
    EXPECT_TRUE(wres[0].empty());
}



TEST(stringTest, split_5)
{// 多个分隔符， 正常情况
    std::string src = "00,, 01,, 02,, 03,, 04";
    std::vector<std::string> res = string::split_vct(src, ",, ");
    EXPECT_EQ(5u, res.size());
    EXPECT_EQ("00", res[0]);
    EXPECT_EQ("01", res[1]);
    EXPECT_EQ("02", res[2]);
    EXPECT_EQ("03", res[3]);
    EXPECT_EQ("04", res[4]);

    std::wstring wsrc = L"00,, 01,, 02,, 03,, 04";
    std::vector<std::wstring> wres = string::split_vct(wsrc, L",, ");
    EXPECT_EQ(5u, res.size());
    EXPECT_EQ(L"00", wres[0]);
    EXPECT_EQ(L"01", wres[1]);
    EXPECT_EQ(L"02", wres[2]);
    EXPECT_EQ(L"03", wres[3]);
    EXPECT_EQ(L"04", wres[4]);
}


TEST(stringTest, split_6)
{// 多个分隔符， 末尾出现分隔符
    std::string src = "00,, 01,, 02,, 03,, 04,, ";
    std::vector<std::string> res = string::split_vct(src, ",, ");
    EXPECT_EQ(6u, res.size());
    EXPECT_EQ("00", res[0]);
    EXPECT_EQ("01", res[1]);
    EXPECT_EQ("02", res[2]);
    EXPECT_EQ("03", res[3]);
    EXPECT_EQ("04", res[4]);
    EXPECT_EQ("", res[5]);
    EXPECT_TRUE(res[5].empty());

    std::wstring wsrc = L"00,, 01,, 02,, 03,, 04,, ";
    std::vector<std::wstring> wres = string::split_vct(wsrc, L",, ");
    EXPECT_EQ(6u, wres.size());
    EXPECT_EQ(L"00", wres[0]);
    EXPECT_EQ(L"01", wres[1]);
    EXPECT_EQ(L"02", wres[2]);
    EXPECT_EQ(L"03", wres[3]);
    EXPECT_EQ(L"04", wres[4]);
    EXPECT_EQ(L"", wres[5]);
    EXPECT_TRUE(wres[5].empty());
}


TEST(stringTest, split_7)
{// 多个分隔符, 中间出现分隔符
    std::string src = "00,, 01,, ,, 02,, 03,, 04";
    std::vector<std::string> res = string::split_vct(src, ",, ");
    EXPECT_EQ(6u, res.size());
    EXPECT_EQ("00", res[0]);
    EXPECT_EQ("01", res[1]);
    EXPECT_EQ("", res[2]);
    EXPECT_EQ("02", res[3]);
    EXPECT_EQ("03", res[4]);
    EXPECT_EQ("04", res[5]);
    EXPECT_TRUE(res[2].empty());

    std::wstring wsrc = L"00,, 01,, ,, 02,, 03,, 04";
    std::vector<std::wstring> wres = string::split_vct(wsrc, L",, ");
    EXPECT_EQ(6u, res.size());
    EXPECT_EQ(L"00", wres[0]);
    EXPECT_EQ(L"01", wres[1]);
    EXPECT_EQ(L"", wres[2]);
    EXPECT_EQ(L"02", wres[3]);
    EXPECT_EQ(L"03", wres[4]);
    EXPECT_EQ(L"04", wres[5]);
    EXPECT_TRUE(wres[2].empty());
}


TEST(stringTest, split_8)
{// 多个分隔符, 开始处出现分隔符
    std::string src = ",, 00,, 01,, 02,, 03,, 04";
    std::vector<std::string> res = string::split_vct(src, ",, ");
    EXPECT_EQ(6u, res.size());
    EXPECT_EQ("", res[0]);
    EXPECT_EQ("00", res[1]);
    EXPECT_EQ("01", res[2]);
    EXPECT_EQ("02", res[3]);
    EXPECT_EQ("03", res[4]);
    EXPECT_EQ("04", res[5]);
    EXPECT_TRUE(res[0].empty());

    std::wstring wsrc = L",, 00,, 01,, 02,, 03,, 04";
    std::vector<std::wstring> wres = string::split_vct(wsrc, L",, ");
    EXPECT_EQ(6u, res.size());
    EXPECT_EQ(L"", wres[0]);
    EXPECT_EQ(L"00", wres[1]);
    EXPECT_EQ(L"01", wres[2]);
    EXPECT_EQ(L"02", wres[3]);
    EXPECT_EQ(L"03", wres[4]);
    EXPECT_EQ(L"04", wres[5]);
    EXPECT_TRUE(wres[0].empty());
}


TEST(stringTest, split)
{// 综合测试
    std::string src = "iegad|28|iegad@vip.qq.com|肖琪";
    std::vector<std::string> res = string::split_vct(src, "|");
    EXPECT_EQ(4u, res.size());
    EXPECT_EQ("iegad", res[0]);
    EXPECT_EQ("28", res[1]);
    EXPECT_EQ("iegad@vip.qq.com", res[2]);
    EXPECT_EQ("肖琪", res[3]);

    std::wstring wsrc = L"iegad|28|iegad@vip.qq.com|肖琪";
    std::vector<std::wstring> wres = string::split_vct(wsrc, L"|");
    EXPECT_EQ(4u, wres.size());
    EXPECT_EQ(L"iegad", wres[0]);
    EXPECT_EQ(L"28", wres[1]);
    EXPECT_EQ(L"iegad@vip.qq.com", wres[2]);
    EXPECT_EQ(L"肖琪", wres[3]);
}


// 2: ============= substr =============

TEST(stringTest, substr)
{// 切割子串， 下标 + 长度
    std::string src = "Hello world";
    EXPECT_EQ("world", string::substr(src, 6, 5));
    EXPECT_EQ("orld", string::substr(src, 7, 1000));
    EXPECT_EQ("orld", string::substr(src, 7, -11));
    EXPECT_EQ("Hello", string::substr(src, 0, 5));
    EXPECT_EQ(string::ERR_STR(), string::substr(src, -1, -101));
    EXPECT_EQ("", string::substr(src, 1, 0));

    std::wstring wsrc = L"Hello world";
    EXPECT_EQ(L"world", string::substr(wsrc, 6, 5));
    EXPECT_EQ(L"orld", string::substr(wsrc, 7, 1000));
    EXPECT_EQ(L"orld", string::substr(wsrc, 7, -11));
    EXPECT_EQ(L"Hello", string::substr(wsrc, 0, 5));
    EXPECT_EQ(string::ERR_WSTR(), string::substr(wsrc, -1, -101));
    EXPECT_EQ(L"", string::substr(wsrc, 1, 0));
}


TEST(stringTest, substr2)
{// 切割子串， 开始下标 + 结束下标
    std::string src = "Hello world";

    EXPECT_EQ("world", string::substr2(src, 6, 11));
    EXPECT_EQ("orld", string::substr2(src, 7, 1011));
    EXPECT_EQ("Hello", string::substr2(src, 0, 5));
    EXPECT_EQ(string::ERR_STR(), string::substr2(src, -1, -101));
    EXPECT_EQ(string::ERR_STR(), string::substr2(src, 5, 5));
    EXPECT_EQ("w", string::substr2(src, 6, 7));

    std::wstring wsrc = L"Hello world";
    EXPECT_EQ(L"world", string::substr2(wsrc, 6, 11));
    EXPECT_EQ(L"orld", string::substr2(wsrc, 7, 1011));
    EXPECT_EQ(L"Hello", string::substr2(wsrc, 0, 5));
    EXPECT_EQ(string::ERR_WSTR(), string::substr2(wsrc, -3, -5));
    EXPECT_EQ(string::ERR_WSTR(), string::substr2(wsrc, 5, 5));
    EXPECT_EQ(L"w", string::substr2(wsrc, 6, 7));
}


// 3: ============= trim =============

TEST(stringTest, rtrim)
{// 右除空字符
    std::string src1 = "hello world\n\t\r  ";
    EXPECT_EQ("hello world", string::rtrim(src1));
    std::string src2 = "hello world\n\t\r  tt";
    EXPECT_NE("hello world", string::rtrim(src2));
    std::string src3 = "hell0 w0rld0000";
    EXPECT_EQ("hell0 w0rld", string::rtrim(src3, '0'));

    std::wstring wsrc1 = L"hello world\n\t\r  ";
    EXPECT_EQ(L"hello world", string::rtrim(wsrc1));
    std::wstring wsrc2 = L"hello world\n\t\r  tt";
    EXPECT_NE(L"hello world", string::rtrim(wsrc2));
    std::wstring wsrc3 = L"hell0 w0rld00";
    EXPECT_EQ(L"hell0 w0rld", string::rtrim(wsrc3, L'0'));
}


TEST(stringTest, trim_1)
{// 除去所有空字符
    std::string src1 = "hello world\n\t\r  ";
    EXPECT_EQ("helloworld", string::trim(src1));
    std::string src2 = "hello world\n\t\r  tt";
    EXPECT_EQ("helloworldtt", string::trim(src2));

    std::wstring wsrc1 = L"hello world\n\t\r  ";
    EXPECT_EQ(L"helloworld", string::trim(wsrc1));
    std::wstring wsrc2 = L"hello world\n\t\r  tt";
    EXPECT_EQ(L"helloworldtt", string::trim(wsrc2));
}


TEST(stringTest, trim_2)
{// 除去所有指定字符
    std::string src1 = "hello world\n\t\r  ";
    EXPECT_EQ("hello world\t\r  ", string::trim(src1, '\n'));
    std::string src2 = "hello world\n\t\r  tt";
    EXPECT_EQ("hello world\t\r  tt", string::trim(src2, '\n'));

    std::wstring wsrc1 = L"hello world\n\t\r  ";
    EXPECT_EQ(L"hello world\t\r  ", string::trim(wsrc1, L'\n'));
    std::wstring wsrc2 = L"hello world\n\t\r  tt";
    EXPECT_EQ(L"hello world\t\r  tt", string::trim(wsrc2, L'\n'));
}


TEST(stringTest, ltrim)
{// 左除空字符
    std::string src1 = "\n\t\r  hello world";
    EXPECT_EQ("hello world", string::ltrim(src1));
    std::string src2 = "\n\t\rhello world  tt";
    EXPECT_EQ("hello world  tt", string::ltrim(src2));
    std::string src3 = "00000hell0 w0rld";
    EXPECT_EQ("hell0 w0rld", string::ltrim(src3, '0'));

    std::wstring wsrc1 = L"\n\t\r  hello world";
    EXPECT_EQ(L"hello world", string::ltrim(wsrc1));
    std::wstring wsrc2 = L"\n\t\rhello world  tt";
    EXPECT_EQ(L"hello world  tt", string::ltrim(wsrc2));
    std::wstring wsrc3 = L"00000hell0 w0rld";
    EXPECT_EQ(L"hell0 w0rld", string::ltrim(wsrc3, L'0'));
}


TEST(stringTest, trim)
{// 综合应用
    std::string src = "\t iegad\n\r  ";
    EXPECT_EQ("iegad", string::rtrim(string::ltrim(src)));

    std::wstring wsrc = L"\t iegad\n\r  ";
    EXPECT_EQ(L"iegad", string::rtrim(string::ltrim(wsrc)));
}


// 4: ============= replace =============

TEST(stringTest, replace)
{// 替换字符串
    std::string src = "Hello world, the world is wonderful";
    EXPECT_EQ("Hello iegad, the iegad is wonderful", string::replace(src, "world", "iegad"));
    EXPECT_EQ(src, string::replace(src, "", "iegad"));
    EXPECT_EQ("Hello , the  is wonderful", string::replace(src, "world", ""));

    std::wstring wsrc = L"Hello world, the world is wonderful";
    EXPECT_EQ(L"Hello iegad, the iegad is wonderful", string::replace(wsrc, L"world", L"iegad"));
    EXPECT_EQ(wsrc, string::replace(wsrc, L"", L"iegad"));
    EXPECT_EQ(L"Hello , the  is wonderful", string::replace(wsrc, L"world", L""));
}


// 5: ============= Find First & Last char's index =============

TEST(stringTest, firstAndLast)
{// 查找指定字符， 第一次或最后一次出现的位置
    std::string src = "Hello world, the world is wonderful";
    EXPECT_EQ(2, string::first_ch(src, 'l'));
    EXPECT_EQ(int(src.size() - 1), string::last_ch(src, 'l'));
    EXPECT_EQ(-1, string::first_ch(src, 'x'));
    EXPECT_EQ(-1, string::last_ch(src, 'x'));
    EXPECT_EQ(-1, string::last_ch(src, '\0'));

    std::wstring wsrc = L"Hello world, the world is wonderful";
    EXPECT_EQ(2, string::first_ch(wsrc, L'l'));
    EXPECT_EQ(int(wsrc.size() - 1), string::last_ch(wsrc, L'l'));
    EXPECT_EQ(-1, string::first_ch(wsrc, L'x'));
    EXPECT_EQ(-1, string::last_ch(wsrc, L'x'));
    EXPECT_EQ(-1, string::last_ch(wsrc, L'\0'));
}


// 6: ============= Find substring =============

TEST(stringTest, find_str)
{// 子字符串查找
    std::string src = "Hello world, the world is wonderful";
    EXPECT_EQ(6, string::find_str(src, "world"));
    EXPECT_EQ(17, string::find_str(src, "world", 2));
    EXPECT_EQ(-1, string::find_str(src, "world", 3));
    EXPECT_EQ(-1, string::find_str(src, "iegad", 1));
    EXPECT_EQ(-1, string::find_str(src, "iegad", 8));
    EXPECT_EQ(0, string::find_str(src, "", 1));
    EXPECT_EQ(-1, string::find_str(src, "", 2));

    std::wstring wsrc = L"Hello world, the world is wonderful";
    EXPECT_EQ(6, string::find_str(wsrc, L"world"));
    EXPECT_EQ(17, string::find_str(wsrc, L"world", 2));
    EXPECT_EQ(-1, string::find_str(wsrc, L"world", 3));
    EXPECT_EQ(-1, string::find_str(wsrc, L"iegad", 1));
    EXPECT_EQ(-1, string::find_str(wsrc, L"iegad", 8));
    EXPECT_EQ(0, string::find_str(wsrc, L"", 1));
    EXPECT_EQ(-1, string::find_str(wsrc, L"", 2));
}


// 7: ============= Start with & End with =============

TEST(stringTest, startWithAndEndWith)
{// 字符串开头与结尾子字符串
    std::string src = "Hello world, the world is wonderful";
    EXPECT_TRUE(string::start_with(src, "Hell"));
    EXPECT_FALSE(string::start_with(src, "Hella"));
    EXPECT_FALSE(string::end_with(src, "swonderful"));
    EXPECT_TRUE(string::end_with(src, " wonderful"));
    EXPECT_TRUE(string::start_with(src, ""));
    EXPECT_TRUE(string::end_with(src, ""));

    std::wstring wsrc = L"Hello world, the world is wonderful";
    EXPECT_TRUE(string::start_with(wsrc, L"Hell"));
    EXPECT_FALSE(string::start_with(wsrc, L"Hella"));
    EXPECT_FALSE(string::end_with(wsrc, L"swonderful"));
    EXPECT_TRUE(string::end_with(wsrc, L" wonderful"));
    EXPECT_TRUE(string::start_with(wsrc, L""));
    EXPECT_TRUE(string::end_with(wsrc, L""));
}


// 8: ============= remove =============

TEST(stringTest, remove_1)
{// 通过 下标 + 长度 删除子符串
    std::string src = "hello world";
    EXPECT_EQ("hello", string::remove(src, 5, 6));
    EXPECT_EQ("hello", string::remove(src, 5, 11));
    EXPECT_EQ("hello", string::remove(src, 5, -6));
    EXPECT_EQ("hello", string::remove(src, 5, 111));
    EXPECT_EQ(src, string::remove(src, -10, -6));
    EXPECT_EQ(src, string::remove(src, -1, 6));
    EXPECT_EQ(src, string::remove(src, 1, 0));

    std::wstring wsrc = L"hello world";
    EXPECT_EQ(L"hello", string::remove(wsrc, 5, 6));
    EXPECT_EQ(L"hello", string::remove(wsrc, 5, 11));
    EXPECT_EQ(L"hello", string::remove(wsrc, 5, 111));
    EXPECT_EQ(L"hello", string::remove(wsrc, 5, -6));
    EXPECT_EQ(wsrc, string::remove(wsrc, -10, -6));
    EXPECT_EQ(wsrc, string::remove(wsrc, -1, 6));
    EXPECT_EQ(wsrc, string::remove(wsrc, 1, 0));
}


TEST(stringTest, remove_2)
{// 通过 开始下标 + 结尾下标 删除子符串
    std::string src = "hello world";
    EXPECT_EQ("hello", string::remove2(src, 5, src.size()));
    EXPECT_EQ("hello", string::remove2(src, 5, 11));
    EXPECT_EQ("hello", string::remove2(src, 5, 111));
    EXPECT_EQ("hello", string::remove2(src, 5, -6));
    EXPECT_EQ(src, string::remove2(src, -10, -6));
    EXPECT_EQ(src, string::remove2(src, -1, 6));
    EXPECT_EQ(src, string::remove2(src, 5, 5));

    std::wstring wsrc = L"hello world";
    EXPECT_EQ(L"hello", string::remove2(wsrc, 5, src.size()));
    EXPECT_EQ(L"hello", string::remove2(wsrc, 5, 11));
    EXPECT_EQ(L"hello", string::remove2(wsrc, 5, 111));
    EXPECT_EQ(L"hello", string::remove2(wsrc, 5, -6));
    EXPECT_EQ(wsrc, string::remove2(wsrc, -10, -6));
    EXPECT_EQ(wsrc, string::remove2(wsrc, -1, 6));
    EXPECT_EQ(wsrc, string::remove2(wsrc, 5, 5));
}


// 9: ============= upper & lower =============

TEST(stringTest, upperAndLower)
{
    std::string src = "abcd12345!@#Aa";
    EXPECT_EQ("ABCD12345!@#AA", string::to_upr(src));
    EXPECT_EQ("abcd12345!@#aa", string::to_lwr(src));

    std::wstring wsrc = L"abcd12345!@#Aa中文 ";
    EXPECT_EQ(L"ABCD12345!@#AA中文 ", string::to_upr(wsrc));
    EXPECT_EQ(L"abcd12345!@#aa中文 ", string::to_lwr(wsrc));
}


// 10: ============= md5 =============

TEST(stringTest, md5)
{
    std::string src = "1234567890里面有文中";
    EXPECT_EQ("b78ce4f329546895258a4161b1900f1e", iegad::security::md5(src));
}


// 11: ============= sha1 =============

TEST(stringTest, sha1)
{
    std::string src = "1234567890里面有文中";
    EXPECT_EQ("073c0a2c33e968da67c9efa506fa8c5c6b886765", iegad::security::sha1(src));
}


// 12: ============= bin 2 str & str 2 bin =============

TEST(stringTest, bin2strAndstr2bin)
{
    std::string src = "1234567890里面有文中";
    std::string en_str = string::bin_tostr(src.c_str(), src.size());
    char buffer[1024] = {0};
    int len = 1024;
    string::str_tobin(en_str, buffer, len);
    EXPECT_EQ(src, std::string(buffer, len));
}


// 13: ============= wstr 2 str & str 2 wstr =============

TEST(stringTest, wstringToStringAndStringToWstring)
{

    std::string src = "1234567890里面有文中";
    std::wstring wstr = string::str_to_wstr(src);
    EXPECT_EQ(L"1234567890里面有文中", wstr);
    EXPECT_EQ(src, string::wstr_to_str(wstr));
}


// 14: ============= format =============
/*
TEST(stringTest, format)
{
    EXPECT_EQ("my name is 肖琪 and 28 years old!我是大天才",
              string::format("my name is {0} and {1} years old!{2}", {"肖琪", std::to_string(28), "我是大天才"}));
    EXPECT_EQ("my name is iegad and iegad is clever",
              string::format("my name is {0} and {0} is {1}", {"iegad", "clever"}));
    EXPECT_EQ("my name is iegad and iegad is clever {2}",
              string::format("my name is {0} and {0} is {1} {2}", {"iegad", "clever"}));
}
*/

// 15: ============= uuid =============

TEST(stringTest, uuid)
{
    std::string src = security::uuid();
    std::cout<<src<<std::endl;
}


// 16: ============= pad =============

TEST(stringTest, pad)
{
    std::string src = "123";
    EXPECT_EQ("000123", string::lpad(src, 3, '0'));
    EXPECT_EQ("123000", string::rpad(src, 3, '0'));

    std::wstring wsrc = L"123";
    EXPECT_EQ(L"000123", string::lpad(wsrc, 3, L'0'));
    EXPECT_EQ(L"123000", string::rpad(wsrc, 3, L'0'));
}


TEST(stringTest, base64)
{
    std::string src = "肖琪是天才";
    std::string enstr = iegad::security::base64_encode(src);
    std::cout<<enstr<<std::endl;
    std::string ret = iegad::security::base64_decode(enstr);
    EXPECT_EQ(src, ret);
}

#if (IEGAD_OPTION_SSL)
TEST(stringTest, aes)
{
    std::string src = "肖琪是天才";
    std::string k = "0987654321";
    std::string enstr = security::aes_encrypt(src, k);
    std::string res = security::aes_decrypt(enstr, k);
    EXPECT_EQ(src, res);
}
#endif //(IEGAD_OPTION_SSL)



















