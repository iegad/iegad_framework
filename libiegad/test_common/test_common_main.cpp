#include <iostream>


#define STRING_TEST	1
#define JOB_TEST		1


#ifdef WIN32
#pragma comment(lib, "gtestd.lib")
#pragma comment(lib, "libglog.lib")
#pragma comment(lib, "libiegad_common.lib")
#endif // WIN32


#if (STRING_TEST == 1)
#include "string_test.hpp"
#endif // #if (STRING_TEST == 1)


#if (JOB_TEST == 1)
#include "job_test.hpp"


std::string TEST_STR[] = {
    "君不见黄河之水天上来, 奔流到海不复还.",
    "君不见高堂明镜悲白发, 朝如青丝暮成雪.",
    "人生得意须尽欢, 莫使金樽空对月.",
    "天生我材必有用, 千金散尽还复来.",
    "烹羊宰牛且为乐, 会须一饮三百杯.",
    "岑夫子, 丹丘生, 将进酒, 君莫停.",
    "与君歌一曲, 请君为我侧耳听.",
    "钟鼓馔玉不足贵, 但愿长醉不愿醒.",
    "古来圣贤皆寂寞, 惟有饮者留其名.",
    "陈王昔时宴平乐, 斗酒十千恣欢谑.",
    "主人何为言少钱, 径须沽取对君酌.",
    "五花马, 千金裘,",
    "呼儿将出换美酒, 与尔同销万古愁."
};


#endif // #if (JOB_TEST == 1)



int 
main(int argc, char *argv[])
{
#if (STRING_TEST == 1)

    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

#endif // #if (STRING_TEST == 1)

#if (JOB_TEST == 1)

    job_test jt(TEST_STR, 13);
    std::cout << "/* ************************************************ */" << std::endl;
    jt.run();
    std::cout << "/* ************************************************ */" << std::endl;


#endif // #if (JOB_TEST == 1)
    std::cout << "test done...\npress<Enter> to exit.";
    std::cin.get();
    exit(0);
}