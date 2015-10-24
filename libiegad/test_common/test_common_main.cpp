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