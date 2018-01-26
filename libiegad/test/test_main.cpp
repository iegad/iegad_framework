#include <iostream>
#include <clocale>
#include <gtest/gtest.h>
#include <iegad/tools/iegad_log.hpp>
#include <iegad/dai/iegad_mysql++.hpp>
#include <iegad/tigen/iegad_tigen_base_handler.hpp>


class MyHandler : public iegad::tigen::TigenBaseHandler<iegad::MySQLDAI> {
public:
    explicit MyHandler(const std::string &initstr) :
        iegad::tigen::TigenBaseHandler<iegad::MySQLDAI>(initstr)
    {}


    virtual ~MyHandler() {}


    void
    setData()
    {
        std::string errstr;
        assert(this->check(&errstr));
        std::cout<<this->reset(&errstr)<<std::endl;

        std::shared_ptr<mysqlpp::Query> q;
        assert(this->get(q, &errstr));
        if (q) {
            std::cout<<"NULL\n";
        }
        assert(q->exec("INSERT INTO T_TEST(T_VAL) VALUES(11)"));
    }

}; // class MyHandler;



int
main(int argc, char* argv[])
{
    std::setlocale(LC_ALL, "zh_CN.UTF-8");
    iegad::tools::_LOG((char *)argv[0]);

    MyHandler handler(iegad::MySQLDAI::makeConnectionString("IEGAD_TEST", "127.0.0.1", "iegad", "1111"));
    handler.setData();

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



/*
#include "filesystem/iegad_filesystem.hpp"
#include "sigar_ex/iegad_sigar.hpp"
int
main()
{
    std::vector<std::string> res = iegad::filesystem::find_files("/usr/local/lib", "lib*");
    for (size_t i = 0; i < res.size(); i++) {
        std::cout<<res[i]<<std::endl;
    }

    iegad::sigar_ex::sigar s;
    std::vector<iegad::sigar_ex::cpu::ptr_t> cpuList = s.getCpuList();
    for (size_t i = 0, n = cpuList.size(); i < n; i++) {
        iegad::sigar_ex::cpu::ptr_t & p = cpuList[i];
        std::cout<<p->getIdle()<<std::endl;
    }

    std::vector<iegad::sigar_ex::cpuInfo::ptr_t> cpuInfoList = s.getCpuInfoList();
    for (size_t i = 0, n = cpuInfoList.size(); i < n; i++) {
        iegad::sigar_ex::cpuInfo::ptr_t & p = cpuInfoList[i];
        std::cout<<p->getVendor()<<'\t'<<p->getModel()<<std::endl;
    }

    std::vector<iegad::sigar_ex::netInterfaceConfig::ptr_t> ifList = s.getNetInterfaceConfigList();
    for (size_t i = 0, n = ifList.size(); i < n; i++) {
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
    for (size_t i = 0, n = ncList.size(); i < n; i++) {
        iegad::sigar_ex::netConnection::ptr_t & p = ncList[i];
        std::cout<<p->getLocalAddress()<<'\t'<<p->getRemoteAddress()<<std::endl;
    }

    iegad::sigar_ex::sysInfo::ptr_t sysInfo = s.getSysInfo();
    std::cout<<sysInfo->getName()<<'\t'<<sysInfo->getMachine()<<'\t'
            <<sysInfo->getVendorVersion()<<std::endl;
    exit(0);
}
*/
