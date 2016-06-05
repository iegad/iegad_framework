#include <iostream>
#include "string/iegad_string.hpp"
#include <clocale>


int
main()
{
    std::setlocale(LC_ALL, "");
    std::cout << iegad::string::wstr_to_str(L"twtwtwererere") << std::endl;
    std::wcout << iegad::string::str_to_wstr("测试的第二段中文字符串") << std::endl;
    std::cout << iegad::string::format("我叫{0}, 今年{1}岁!!!{1}岁了, 可是我还是没有找到自己的事业!!!", { "肖琪", std::to_string(29) });
    exit(0);
}
