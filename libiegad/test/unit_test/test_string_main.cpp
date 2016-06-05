#include <iostream>
#include "string/iegad_string.h"
#include <clocale>


int
main()
{
    std::setlocale(LC_ALL, "");
    std::cout << iegad::string::wstr_to_str(L"���������ַ���") << std::endl;
    std::wcout << iegad::string::str_to_wstr("���Եĵڶ��������ַ���") << std::endl;
    std::cout << iegad::string::format("�ҽ�{0}, ����{1}��!!!{1}����, �����һ���û���ҵ��Լ�����ҵ!!!", { "Ф��", std::to_string(29) }) << std::endl;
    std::string temp = iegad::string::sha1("iegad");
    if ("98dd2bbbb974a5f82e4667480f07a03de0768f7d" == temp) {
	std::cout << "good\n";
    }
    std::cin.get();
    exit(0);
}
