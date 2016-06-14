#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/connpool/connect_manager.hpp"

namespace acl {

class ACL_CPP_API sqlite_manager : public connect_manager
{
public:
	/**
	 * ���캯��
	 * @param charset {const char*} �����ļ����ַ���
	 */
	sqlite_manager(const char* charset = "utf-8");
	~sqlite_manager();

	/**
	* @param dbfile {const char*} sqlite ���ݿ�������ļ�
	* @param dblimit {size_t} ���ݿ����ӳ��������������
	* @return {sqlite_manager&}
	 */
	sqlite_manager& add(const char* dbfile, size_t dblimit);

protected:
	/**
	 * ���� connect_manager �麯����ʵ��
	 * @param addr {const char*} ������������ַ����ʽ��ip:port
	 * @param count {size_t} ���ӳصĴ�С���ƣ���ֵΪ 0 ʱû������
	 * @param idx {size_t} �����ӳض����ڼ����е��±�λ��(�� 0 ��ʼ)
	 * @return {connect_pool*} ���ش��������ӳض���
	 */
	connect_pool* create_pool(const char* addr, size_t count, size_t idx);

private:
	// sqlite �����ļ���
	char* dbfile_;
	char* charset_;
	size_t dblimit_;
};

} // namespace acl
