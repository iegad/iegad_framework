#include <iostream>
#include <boost/locale.hpp>


#define STRING_TEST		1
#define JOB_TEST			1
#define LOG_TEST			1
#define SINGLETON_TEST	1
#define TIMER_TEST			1
#define SAFE_MAP_TEST		1
#define PUGIXML_TEST		1		



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


#if (LOG_TEST == 1)
#include "log_test.hpp"
#endif


#if (SINGLETON_TEST == 1)
#include "singleton_test.hpp"
#endif


#if (TIMER_TEST == 1)
#include "timer_test.hpp"
#endif


#if (SAFE_MAP_TEST == 1)
#include "safe_map_test.hpp"
#endif


#if (PUGIXML_TEST == 1)
#include "pugixml_test.hpp"
#endif


int 
main(int argc, char *argv[])
{
#if (STRING_TEST == 1) // 字符串算法测试
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    //std::string srcStr = "Hello world";
    //std::string basestr = iegad::string::base64_en(srcStr.c_str(), srcStr.size());
    //std::string outStr = iegad::string::base64_de(basestr);
#endif // #if (STRING_TEST == 1)


#if (JOB_TEST == 1) // 任务队列-工作模型测试
    job_test jt(TEST_STR, 13);
    std::cout << "/* ************************************************ */" << std::endl;
    jt.run();
    std::cout << "/* ************************************************ */" << std::endl;
#endif // #if (JOB_TEST == 1)


#if (LOG_TEST == 1) // 日志测试
    log_test test(argv[0]);
    iWARN << "This is a Warnning message" << std::endl;
    iERR << "This is a Error message" << std::endl;
    iINFO << "This is a Info message" << std::endl;
#endif // #if (LOG_TEST == 1) 


#if (SINGLETON_TEST == 1) // 线程安全的单例测试
    singleton_test();
#endif // #if (SINGLETON_TEST == 1)


#if (TIMER_TEST == 1) // 定时器测试
    timer_test("2015-11-07 18:42:00");
#endif // #if (SINGLETON_TEST == 1)


#if (PUGIXML_TEST == 1) // XML
    if (!create_xmldoc("app.config")) {
	std::cout << "[ CREATE XML DOCUMENT ] : [ FAILED ]\n";
	goto TEST_END;
    }

    if (!load_xmldoc("app.config")) {
	std::cout << "[ LOAD XML DOCUMENT ] : [ FAILED ]\n";
    }

    TEST_END:
#endif

    std::cout << "test done...\npress<Enter> to exit.";
    std::cin.get();
    exit(0);
}
