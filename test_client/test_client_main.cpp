#include "echo_proxy.h"
#include <iostream>
#include <codecvt>




enum {
    N_TIMES = 100
};



int
main(int argc, char * argv[])
{
    int nCount = 0;
    std::locale::global(std::locale("chs"));

    for (int i = 0; i < N_TIMES; i++) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	// �����ͳ���Ϊ127ʱ, �����޾���, ����, protocol buffer�ᷢ������
	std::string rzt;
	rzt = echo_proxy("127.0.0.1", "6688")("Ф���ǳ�������� Ф���ǳ�������� Ф���ǳ�������� Ф���ǳ�������� Ф���ǳ��������!!!!");
	std::cout << rzt << std::endl;
	nCount++;
    }
    std::cout << nCount << " times done" << std::endl;
exit_case:
    std::cin.get();
    exit(0);
}