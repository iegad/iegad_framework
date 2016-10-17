#include <iostream>
#include <clocale>
#include <locale>
#include <gtest/gtest.h>
#include "sercurity/iegad_aes.hpp"
#include "sigar_ex/iegad_sigar.hpp"
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
    std::vector<std::string> res = iegad::filesystem::find_files("/usr/local/lib", "lib*");
    for (int i = 0; i < res.size(); i++) {
        std::cout<<res[i]<<std::endl;
    }

    iegad::sigar_ex::sigar s;
    std::vector<iegad::sigar_ex::cpu::ptr_t> cpuList = s.getCpuList();
    for (int i = 0, n = cpuList.size(); i < n; i++) {
        iegad::sigar_ex::cpu::ptr_t & p = cpuList[i];
        std::cout<<p->getIdle()<<std::endl;
    }

    std::vector<iegad::sigar_ex::cpuInfo::ptr_t> cpuInfoList = s.getCpuInfoList();
    for (int i = 0, n = cpuInfoList.size(); i < n; i++) {
        iegad::sigar_ex::cpuInfo::ptr_t & p = cpuInfoList[i];
        std::cout<<p->getVendor()<<'\t'<<p->getModel()<<std::endl;
    }

    std::vector<iegad::sigar_ex::netInterfaceConfig::ptr_t> ifList = s.getNetInterfaceConfigList();
    for (int i = 0, n = ifList.size(); i < n; i++) {
        iegad::sigar_ex::netInterfaceConfig::ptr_t & p = ifList[i];
        std::cout<<p->getAddress()<<'\t'
                <<p->getBroadCast()<<'\t'
               <<p->getAddress6()<<'\t'
              <<p->getNetMask()<<'\t'
             <<p->getDescription()<<'\t'
              <<p->getHwaddr()<<std::endl;
    }


    std::vector<iegad::sigar_ex::netConnection::ptr_t> ncList = s.getNetConnectionList(-1);
    std::cout<<"[net connection]\n";
    for (int i = 0, n = ncList.size(); i < n; i++) {
        iegad::sigar_ex::netConnection::ptr_t & p = ncList[i];
        std::cout<<p->getLocalAddress()<<'\t'<<p->getRemoteAddress()<<std::endl;
    }

    iegad::sigar_ex::sysInfo::ptr_t sysInfo = s.getSysInfo();
    std::cout<<sysInfo->getName()<<'\t'<<sysInfo->getMachine()<<'\t'
            <<sysInfo->getVendorVersion()<<std::endl;
    exit(0);
}
