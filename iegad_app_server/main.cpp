#include "iegad_framework.h"
#include "iegad_app_svr.h"
#include "setup_svc.h"


#include <iostream>





int 
main(int argc, char * argv[])
{
    app_svr host("127.0.0.1", 6688, argv[0]);
    host.run();


    std::cout << "press <Enter> to exit..." << std::endl;
    std::cin.get();
    exit(0);
}