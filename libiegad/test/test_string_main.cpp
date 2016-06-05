#include <iostream>
#include "string/iegad_string.hpp"
#include <clocale>


int
main()
{
    std::setlocale(LC_ALL, "");
    std::cout << iegad::string::wstr_to_str(L"twtwtwererere") << std::endl;
    std::wcout << iegad::string::str_to_wstr("���Եĵڶ��������ַ���") << std::endl;
    std::cout << iegad::string::format("�ҽ�{0}, ����{1}��!!!{1}����, �����һ���û���ҵ��Լ�����ҵ!!!", { "Ф��", std::to_string(29) });
    exit(0);
}
