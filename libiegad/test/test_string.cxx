#include <iostream>
#include <clocale>

#include <gtest/gtest.h>

#include "string/iegad_string.hpp"
#include "tools/iegad_log.hpp"


using namespace iegad;


TEST(stringTest, split)
{
    std::string src = "This,is, a, Hello world, testing of , split";
    std::wstring wsrc = L"This,is, a, Hello world, testing of , split";

    auto strVct = string_ex::split_vct(src, ",");
    EXPECT_EQ(6, strVct.size());
    EXPECT_EQ("This", strVct[0]);
    EXPECT_EQ("is", strVct[1]);
    EXPECT_EQ(" a", strVct[2]);
    EXPECT_EQ(" Hello world", strVct[3]);
    EXPECT_EQ(" testing of ", strVct[4]);
    EXPECT_EQ(" split", strVct[5]);

    strVct = string_ex::split_vct(src, ", ");
    EXPECT_EQ(5, strVct.size());
    EXPECT_EQ("This,is", strVct[0]);
    EXPECT_EQ("a", strVct[1]);
    EXPECT_EQ("Hello world", strVct[2]);
    EXPECT_EQ("testing of ", strVct[3]);
    EXPECT_EQ("split", strVct[4]);

    auto wstrVct = string_ex::split_vct(wsrc, L",");
    EXPECT_EQ(6, wstrVct.size());
    EXPECT_EQ(L"This", wstrVct[0]);
    EXPECT_EQ(L"is", wstrVct[1]);
    EXPECT_EQ(L" a", wstrVct[2]);
    EXPECT_EQ(L" Hello world", wstrVct[3]);
    EXPECT_EQ(L" testing of ", wstrVct[4]);
    EXPECT_EQ(L" split", wstrVct[5]);

    wstrVct = string_ex::split_vct(wsrc, L", ");
    EXPECT_EQ(5, wstrVct.size());
    EXPECT_EQ(L"This,is", wstrVct[0]);
    EXPECT_EQ(L"a", wstrVct[1]);
    EXPECT_EQ(L"Hello world", wstrVct[2]);
    EXPECT_EQ(L"testing of ", wstrVct[3]);
    EXPECT_EQ(L"split", wstrVct[4]);
}


TEST(stringTest, substr)
{
    std::string src = "Hello world";
    std::wstring wsrc = L"Hello world";

    EXPECT_EQ("Hello", string_ex::substr(src, 0, 5));
    EXPECT_EQ("world", string_ex::substr(src, 6));
    EXPECT_EQ("ello worl", string_ex::substr(src, 1, 9));

    EXPECT_EQ(L"Hello", string_ex::substr(wsrc, 0, 5));
    EXPECT_EQ(L"world", string_ex::substr(wsrc, 6));
    EXPECT_EQ(L"ello worl", string_ex::substr(wsrc, 1, 9));
}


TEST(stringTest, trim_family)
{
    std::string src = "\nThis \tis \ta \ttesting \twords\n";
    std::wstring wsrc = L"\nThis \tis \ta \ttesting \twords\n";

    EXPECT_EQ("\nThis \tis \ta \ttesting \twords", string_ex::rtrim(src));
    EXPECT_EQ("This \tis \ta \ttesting \twords\n", string_ex::ltrim(src));
    EXPECT_EQ("Thisisatestingwords", string_ex::trim(src));
    EXPECT_EQ("\nThis is a testing words\n", string_ex::trim(src, '\t'));

    EXPECT_EQ(L"\nThis \tis \ta \ttesting \twords", string_ex::rtrim(wsrc));
    EXPECT_EQ(L"This \tis \ta \ttesting \twords\n", string_ex::ltrim(wsrc));
    EXPECT_EQ(L"Thisisatestingwords", string_ex::trim(wsrc));
    EXPECT_EQ(L"\nThis is a testing words\n", string_ex::trim(wsrc, L'\t'));
}


TEST(stringTest, replace)
{
    std::string src = "Hello world";
    std::wstring wsrc = L"Hello world";

    EXPECT_EQ("Hello iegad", string_ex::replace(src, "world", "iegad"));
    EXPECT_EQ(L"Hello iegad", string_ex::replace(wsrc, L"world", L"iegad"));
}


TEST(stringTest, search)
{
    std::string src = "Hello world";
    std::wstring wsrc = L"Hello world";

    EXPECT_EQ(2, string_ex::first_ch(src, 'l'));
    EXPECT_EQ(9, string_ex::last_ch(src, 'l'));
    EXPECT_EQ(7, string_ex::find_str(src, "o", 2));
    EXPECT_EQ(true, string_ex::start_with(src, "Hel"));
    EXPECT_EQ(false, string_ex::start_with(src, "Helo"));
    EXPECT_EQ(true, string_ex::end_with(src, "rld"));
    EXPECT_EQ(false, string_ex::end_with(src, "rl"));

    EXPECT_EQ(2, string_ex::first_ch(wsrc, L'l'));
    EXPECT_EQ(9, string_ex::last_ch(wsrc, L'l'));
    EXPECT_EQ(7, string_ex::find_str(wsrc, L"o", 2));
    EXPECT_EQ(true, string_ex::start_with(wsrc, L"Hel"));
    EXPECT_EQ(false, string_ex::start_with(wsrc, L"Helo"));
    EXPECT_EQ(true, string_ex::end_with(wsrc, L"rld"));
    EXPECT_EQ(false, string_ex::end_with(wsrc, L"rl"));
}


TEST(stringTest, removing)
{
    std::string src = "Hello world";
    std::wstring wsrc = L"Hello world";

    EXPECT_EQ("world", string_ex::remove(src, 0, 6));
    EXPECT_EQ("Hello", string_ex::remove(src, 5));
    EXPECT_EQ("He world", string_ex::remove2(src, 2, 4));
    EXPECT_EQ("Hello", string_ex::remove2(src, 5));

    EXPECT_EQ(L"world", string_ex::remove(wsrc, 0, 6));
    EXPECT_EQ(L"Hello", string_ex::remove(wsrc, 5));
    EXPECT_EQ(L"He world", string_ex::remove2(wsrc, 2, 4));
    EXPECT_EQ(L"Hello", string_ex::remove2(wsrc, 5));
}


TEST(stringTest, transform)
{
    std::string src = "Hello world";
    std::wstring wsrc = L"Hello world";

    EXPECT_EQ("hello world", string_ex::to_lwr(src));
    EXPECT_EQ("HELLO WORLD", string_ex::to_upr(src));
    EXPECT_EQ("iegad is very clever", string_ex::format("{0} is {1} {2}", {"iegad", "very", "clever"}));

    EXPECT_EQ(L"hello world", string_ex::to_lwr(wsrc));
    EXPECT_EQ(L"HELLO WORLD", string_ex::to_upr(wsrc));
    EXPECT_EQ(L"iegad is very clever", string_ex::format(L"{0} is {1} {2}", {L"iegad", L"very", L"clever"}));
}


TEST(stringTest, encoding)
{
    std::string src = "Hello world";
    std::string buff = "测试自符串";

    char debuff[256] = {0};
    int debufsize = 256;

    EXPECT_EQ("3e25960a79dbc69b674cd4ec67a72c62", string_ex::md5(src));
    EXPECT_EQ("7b502c3a1f48c8609ae212cdfb639dee39673f5e", string_ex::sha1(src));

    EXPECT_EQ("702d0c4cc3fd54d94b42444fd17cf4d7", string_ex::md5("肖琪"));
    EXPECT_EQ("625b63b3ab4bae771a9731eb59a785cb6b36f577", string_ex::sha1("肖琪"));

    auto enstr = string_ex::bin_tostr(buff.c_str(), buff.size());
    EXPECT_EQ("测试自符串", std::string(string_ex::str_tobin(enstr, debuff, debufsize), debufsize));

    EXPECT_EQ(L"Hello world", string_ex::str_to_wstr(src));
    EXPECT_EQ(src, string_ex::wstr_to_str(L"Hello world"));
}



