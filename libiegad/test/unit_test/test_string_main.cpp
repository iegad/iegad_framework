#include <iostream>
#include "string/iegad_string.h"
#include <clocale>


int
main()
{
    std::setlocale(LC_ALL, "");
    std::cout << iegad::string::wstr_to_str(L"���������ַ���") << std::endl;
    std::wcout << iegad::string::str_to_wstr("���Եĵڶ��������ַ���") << std::endl;
    std::cout << iegad::string::format("�ҽ�{0}, ����{1}��!!!{1}����, �����һ���û���ҵ��Լ�����ҵ!!!", { "Ф��", std::to_string(29) });
    std::cin.get();
    exit(0);
}
