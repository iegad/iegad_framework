#include <iostream>
#include "string/iegad_string.hpp"
#include <clocale>


int
main()
{
    std::setlocale(LC_ALL, "");
    std::cout << iegad::string::wstr_to_str(L"twtwtwererere") << std::endl;
    std::wcout << iegad::string::str_to_wstr("���Եĵڶ��������ַ���") << std::endl;
<<<<<<< HEAD:libiegad/test/unit_test/test_string_main.cpp
    std::cout << iegad::string::format("�ҽ�{0}, ����{1}��!!!{1}����, �����һ���û���ҵ��Լ�����ҵ!!!", { "Ф��", std::to_string(29) }) << std::endl;
    std::string temp = iegad::string::sha1("iegad");
    if ("98dd2bbbb974a5f82e4667480f07a03de0768f7d" == temp) {
	std::cout << "good\n";
    }
    std::cin.get();
=======
    std::cout << iegad::string::format("�ҽ�{0}, ����{1}��!!!{1}����, �����һ���û���ҵ��Լ�����ҵ!!!", { "Ф��", std::to_string(29) });
>>>>>>> 060fdfb35a03e1e62f7edcd4a0ed1371e84aaf1d:libiegad/test/test_string_main.cpp
    exit(0);
}
