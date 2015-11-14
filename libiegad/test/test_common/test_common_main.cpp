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
    "�������ƺ�֮ˮ������, ��������������.",
    "�����������������׷�, ������˿ĺ��ѩ.",
    "���������뾡��, Īʹ���׿ն���.",
    "�����Ҳı�����, ǧ��ɢ��������.",
    "������ţ��Ϊ��, ����һ�����ٱ�.",
    "᯷���, ������, ������, ��Īͣ.",
    "�����һ��, ���Ϊ�Ҳ����.",
    "�ӹ��������, ��Ը����Ը��.",
    "����ʥ�ͽԼ�į, Ω������������.",
    "������ʱ��ƽ��, ����ʮǧ����.",
    "���˺�Ϊ����Ǯ, �����ȡ�Ծ���.",
    "�廨��, ǧ����,",
    "��������������, ���ͬ����ų�."
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
#if (STRING_TEST == 1) // �ַ����㷨����
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    //std::string srcStr = "Hello world";
    //std::string basestr = iegad::string::base64_en(srcStr.c_str(), srcStr.size());
    //std::string outStr = iegad::string::base64_de(basestr);
#endif // #if (STRING_TEST == 1)


#if (JOB_TEST == 1) // �������-����ģ�Ͳ���
    job_test jt(TEST_STR, 13);
    std::cout << "/* ************************************************ */" << std::endl;
    jt.run();
    std::cout << "/* ************************************************ */" << std::endl;
#endif // #if (JOB_TEST == 1)


#if (LOG_TEST == 1) // ��־����
    log_test test(argv[0]);
    iWARN << "This is a Warnning message" << std::endl;
    iERR << "This is a Error message" << std::endl;
    iINFO << "This is a Info message" << std::endl;
#endif // #if (LOG_TEST == 1) 


#if (SINGLETON_TEST == 1) // �̰߳�ȫ�ĵ�������
    singleton_test();
#endif // #if (SINGLETON_TEST == 1)


#if (TIMER_TEST == 1) // ��ʱ������
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
