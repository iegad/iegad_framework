#include <iostream>
#include <clocale>
#include <locale>
#include <gtest/gtest.h>
#include "sercurity/iegad_aes.hpp"
#include "filesystem/iegad_filesystem.hpp"


/*
int
main(int argc, char * argv[])
{    
    std::setlocale(LC_ALL, "zh_CN.UTF-8");                        
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/




int
main()
{
    std::cout<<iegad::filesystem::find_file("/usr/local/bin/", "ab")<<std::endl;

    std::vector<iegad::filesystem::fileInfo> filist = iegad::filesystem::ls("/");

    for (int i = 0, n = filist.size(); i < n; i++) {
        std::cout<<filist[i].name<<'\t'
                <<filist[i].used<<'\t'
               <<filist[i].capacity<<std::endl;
    }



    exit(0);
}
