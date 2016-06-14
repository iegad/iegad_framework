#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include <map>
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/connpool/connect_manager.hpp"

namespace acl {

class mysql_conf;

class ACL_CPP_API mysql_manager : public connect_manager
{
public:
	mysql_manager(time_t idle_ttl = 120);
	~mysql_manager();

	/**
	 * ���һ�����ݿ�ʵ������һ
	 * @param dbaddr {const char*} mysql ��������ַ����ʽ��IP:PORT��
	 *  �� UNIX ƽ̨�¿���Ϊ UNIX ���׽ӿ�
	 * @param dbname {const char*} ���ݿ���
	 * @param dbuser {const char*} ���ݿ��û�
	 * @param dbpass {const char*} ���ݿ��û�����
	 * @param dblimit {size_t} ���ݿ����ӳص��������������
	 * @param dbflags {unsigned long} mysql ���λ
	 * @param auto_commit {bool} �Ƿ��Զ��ύ
	 * @param conn_timeout {int} �������ݿⳬʱʱ��(��)
	 * @param rw_timeout {int} �����ݿ�ͨ��ʱ��IOʱ��(��)
	 * @param charset {const char*} �������ݿ�ʱ���ַ���
	 * @return {mysql_manager&}
	 */
	mysql_manager& add(const char* dbaddr, const char* dbname,
		const char* dbuser, const char* dbpass,
		size_t dblimit = 64, unsigned long dbflags = 0,
		bool auto_commit = true, int conn_timeout = 60,
		int rw_timeout = 60, const char* charset = "utf8");

	/**
	 * ���һ�����ݿ�ʵ��������
	 * @param conf {const mysql_conf&}
	 * @return {mysql_manager&}
	 */
	mysql_manager& add(const mysql_conf& conf);

protected:
	/**
	 * ���� connect_manager �麯����ʵ��
	 * @param addr {const char*} ������������ַ����ʽ��ip:port
	 * @param count {size_t} ���ӳصĴ�С���ƣ���ֵΪ 0 ʱ��û������
	 * @param idx {size_t} �����ӳض����ڼ����е��±�λ��(�� 0 ��ʼ)
	 * @return {connect_pool*} ���ش��������ӳض���
	 */
	connect_pool* create_pool(const char* addr, size_t count, size_t idx);

private:
	time_t idle_ttl_;       // ���ݿ����ӵĿ��й���ʱ��
	std::map<string, mysql_conf*> dbs_;
};

} // namespace acl
