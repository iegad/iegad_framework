#include "echo_proxy.h"
#include <iostream>
#include <codecvt>
#include "net/udp_puller.h"



#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif



enum {
    N_TIMES = 10000
};


void 
test_proc()
{
    std::string outstr;
    for (int i = 0; i < N_TIMES; i++) {
	outstr = echo_proxy("127.0.0.1", "6688")("Hello");
	if (outstr == "") {
	    std::cout << "failed" << std::endl;
	}
    }
}


void
fun(char c)
{
    static int i = 1;
    std::cout << i++ << "\t\t" << (int)c << std::endl;
}


int
main(int argc, char * argv[])
{
   
    std::string instr, outstr;
    clock_t start, finished;
    // ================ 测试专用 ===================
 //   std::cout << "Press <Enter> to start..." << std::endl;
 //   std::cin.get();
 //   std::cout << "Starting ...\n";
 //   start = clock();



 //   std::vector<std::thread> thread_pool;
 //   for (int i = 0; i < 1; i++) {
	//thread_pool.push_back(std::thread(test_proc));
 //   }

 //   for (int i = 0; i < 1; i++) {
	//thread_pool[i].join();
 //   }
 //   finished = clock();
 //   std::cout << "use : " << (finished - start) / CLOCKS_PER_SEC << std::endl;

    // ================ 测试专用 ===================


 //   std::cout << "Enter the str & 'exit' to exit..." << std::endl;
 //   do {

	//std::getline(std::cin, instr, '\n');
	//if ("exit" == iegad::string::to_lwr(instr)) {
	//    break;
	//}
	//std::cout << "echo :" << echo_proxy("192.168.1.104", "6688")(instr) << std::endl;
 //   } while (true);

    const std::string str = "abc_abc_abcd";
    std::string tem(str.begin(), str.begin() + 5);

    //iegad::net::udp_puller wtr("127.0.0.1", 12013, fun);
    //wtr.start();



exit_case:
    _CrtDumpMemoryLeaks();
    std::cin.get();
    exit(0);
}