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
	// 当发送长度为127时, 发送无警告, 否则, protocol buffer会发出警告
	std::string rzt;
	rzt = echo_proxy("127.0.0.1", "6688")("肖琪是超级大天才 肖琪是超级大天才 肖琪是超级大天才 肖琪是超级大天才 肖琪是超级大天才!!!!");
	std::cout << rzt << std::endl;
	nCount++;
    }
    std::cout << nCount << " times done" << std::endl;
exit_case:
    std::cin.get();
    exit(0);
}