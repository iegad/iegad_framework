#include "common/iegad_string.h"
#include <iostream>
#include "gtest/gtest.h"


#ifdef WIN32
#pragma comment(lib, "gtestd.lib")
#pragma comment(lib, "libglog.lib")
#pragma comment(lib, "libiegad_common.lib")
#endif // WIN32




TEST(STR_UTEST_CASE, SUBSTR_TEST)
{
    std::string srcString = "0123456789";
    EXPECT_EQ("345678", iegad::string::substr(srcString, 3, 6));
    EXPECT_EQ("56789", iegad::string::substr(srcString, 5));
    EXPECT_EQ("456789", iegad::string::substr2(srcString, 4));
    EXPECT_EQ("4567", iegad::string::substr2(srcString, 4, 7));
}


TEST(STR_UTEST_CASE, TRIM_TEST)
{
    std::string srcString = "   01234   56789    \t\t";
    std::string srcString2 = ",,1, 2, 3, 4, 5, 6, 7, 8,,";
    EXPECT_EQ("0123456789", iegad::string::trim(srcString));
    EXPECT_EQ("01234   56789", iegad::string::ltrim(iegad::string::rtrim(srcString)));
    EXPECT_EQ("0123456789\t\t", iegad::string::trim(srcString, ' '));
    EXPECT_EQ("0123456789", iegad::string::trim(iegad::string::trim(srcString, ' '), '\t'));
    EXPECT_EQ("12345678", iegad::string::trim(iegad::string::trim(srcString2), ','));
}


TEST(STR_UTEST_CASE, REPLACE_TEST)
{
    std::string srcString = "iegad is my name, iegad is very clever";
    EXPECT_EQ("Xiaoqi is my name, Xiaoqi is very clever", iegad::string::replace(srcString, "iegad", "Xiaoqi"));
    EXPECT_EQ("Aizen is my name, Aizen is very clever", iegad::string::replace(srcString, "iegad ", "Aizen "));
    int n = iegad::string::fstchr(srcString, ',');
    EXPECT_EQ("iegad is my name", iegad::string::remove(srcString, n));
    n = iegad::string::find_str(srcString, " is");
    int end = iegad::string::lstchr(srcString, 'd');
    EXPECT_EQ("iegad is very clever", iegad::string::remove2(srcString, n, end));
}


TEST(STR_UTEST_CASE, FIND_TEST)
{
    std::string srcString = "iegad is my name, iegad is very clever";
    EXPECT_EQ(3, iegad::string::fstchr(srcString, 'a'));
    EXPECT_EQ(35, iegad::string::lstchr(srcString, 'v'));
    EXPECT_EQ(0, iegad::string::find_str(srcString, "iegad"));
    EXPECT_EQ(18, iegad::string::find_str(srcString, "iegad", 2));
    EXPECT_EQ(true, iegad::string::start_with(srcString, "iega"));
    EXPECT_EQ(false, iegad::string::start_with(srcString, "iegadd"));
    EXPECT_EQ(false, iegad::string::end_with(srcString, "ever "));
    EXPECT_EQ(true, iegad::string::end_with(srcString, ", iegad is very clever"));
}


TEST(STR_UTEST_CASE, SPLIT_TEST)
{
    std::string srcString = "Aizen, iegad, Xiaoqi, , XiaoQ";
    std::vector<std::string> strVct = std::move(iegad::string::split_vct(srcString, ", "));
    EXPECT_EQ(5, strVct.size());
    EXPECT_EQ("Aizen", strVct[0]);
    EXPECT_EQ("iegad", strVct[1]);
    EXPECT_EQ("Xiaoqi", strVct[2]);
    EXPECT_EQ("", strVct[3]);
    EXPECT_EQ("XiaoQ", strVct[4]);
    srcString = iegad::string::trim(srcString);
    strVct.clear();
    strVct = std::move(iegad::string::split_vct(srcString, ","));
    EXPECT_EQ("Aizen", strVct[0]);
    EXPECT_EQ("iegad", strVct[1]);
    EXPECT_EQ("Xiaoqi", strVct[2]);
    EXPECT_EQ("", strVct[3]);
    EXPECT_EQ("XiaoQ", strVct[4]);
}



int 
main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    std::cin.get();
    exit(0);
}