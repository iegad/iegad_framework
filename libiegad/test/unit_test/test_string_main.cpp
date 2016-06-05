#include <iostream>
#include "string/iegad_string.h"
#include <clocale>


int
main()
{
    std::setlocale(LC_ALL, "");
    std::cout << iegad::string::wstr_to_str(L"测试中文字符串") << std::endl;
    std::wcout << iegad::string::str_to_wstr("测试的第二段中文字符串") << std::endl;
    std::cout << iegad::string::format("我叫{0}, 今年{1}岁!!!{1}岁了, 可是我还是没有找到自己的事业!!!", { "肖琪", std::to_string(29) }) << std::endl;
    std::string temp = iegad::string::sha1("iegad");
    if ("98dd2bbbb974a5f82e4667480f07a03de0768f7d" == temp) {
	std::cout << "good\n";
    }
    std::cin.get();
    exit(0);
}
