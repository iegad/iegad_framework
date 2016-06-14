#pragma once
#include "acl_cpp_define.hpp"

namespace acl
{

	/**
	 * �� _WIN32 dos �����£������Ҫʹ���׽ӿڲ�����
	 * ����Ҫ�ȵ��ô˺������г�ʼ��
	 */
	ACL_CPP_API void acl_cpp_init(void);

#if defined(_WIN32) || defined(_WIN64)
	/**
	 * win32 �´� DOS ����
	 */
	ACL_CPP_API void open_dos(void);

	/**
	 * win32 �¹ر� DOS ����
	 */
	ACL_CPP_API void close_dos(void);
#endif

}
