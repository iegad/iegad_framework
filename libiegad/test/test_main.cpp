#include <iostream>
#include <clocale>
#include <locale>
#include <gtest/gtest.h>
#include "sercurity/iegad_aes.hpp"



int
main(int argc, char * argv[])
{    
    std::setlocale(LC_ALL, "zh_CN.UTF-8");                        
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
