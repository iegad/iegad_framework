#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include <map>
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/session/session.hpp"

namespace acl
{

class redis;
class redis_client_cluster;

class ACL_CPP_API redis_session : public session
{
public:
	redis_session(redis_client_cluster& cluster, size_t max_conns,
		time_t ttl = 0, const char* sid = NULL);
	~redis_session();

	// �����麯������ redis ��������ù�ϣ����ֵ
	bool set(const char* name, const char* value);

	// �����麯������ redis ��������ù�ϣ����ֵ
	bool set(const char* name, const void* value, size_t len);

	// �����麯������ redis ����˵Ĺ�ϣ�����л�ö�Ӧ���Ե�ֵ
	const session_string* get_buf(const char* name);

	// �����麯������ redis ����˵Ĺ�ϣ������ɾ��ĳ������ֵ
	bool del(const char* name);

	// ���ി�麯������ redis ��ɾ������
	bool remove();

	// ���ി�麯������ redis �л������
	bool get_attrs(std::map<string, session_string>& attrs);

	// �����麯������ redis �л������
	bool get_attrs(const std::vector<string>& names,
		std::vector<session_string>& values);

	// ���ി�麯������ redis ����ӻ��޸�����
	bool set_attrs(const std::map<string, session_string>& attrs);

protected:
	//�������� session �� redis �ϵĻ���ʱ��
	bool set_timeout(time_t ttl);

private:
	redis_client_cluster& cluster_;
	redis* command_;
	size_t max_conns_;
	std::map<string, session_string*> buffers_;
};

} // namespace acl
