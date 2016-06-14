#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/connpool/connect_pool.hpp"

namespace acl
{

class polarssl_conf;

/**
 * http �ͻ������ӳ��࣬���ุ��Ϊ connect_pool������ֻ��ʵ�ָ����е��麯��
 * create_connect ��ӵ�������ӳظ��� connect_pool �Ĺ��ܣ����⣬���ഴ��
 * �����Ӷ����� http_reuqest ���������ڵ��� connect_pool::peek ʱ����
 * �ı��� http_request �࣬��������Ҫ�� peek ���ص������ǿ��תΪ http_request
 * ����󣬱����ʹ�� http_request �������й��ܣ����� http_reuqest ��Ϊ
 * connect_client ������
 */
class ACL_CPP_API http_request_pool : public connect_pool
{
public:
	/**
	 * ���캯��
	 * @param addr {const char*} ������������ַ����ʽ��ip:port(domain:port)
	 * @param count {size_t} ���ӳ�������Ӹ������ƣ�����ֵΪ 0 ʱ��û������
	 * @param idx {size_t} �����ӳض����ڼ����е��±�λ��(�� 0 ��ʼ)
	 */
	http_request_pool(const char* addr, size_t count, size_t idx = 0);
	~http_request_pool();

	/**
	 * ���ñ��������� SSL �Ŀͻ���ģʽ
	 * @param ssl_conf {polarssl_conf*}
	 */
	void set_ssl(polarssl_conf* ssl_conf);

protected:
	// ���ി�麯�����ú������غ��ɻ������ø����ӳص��������Ӽ����� IO ��ʱʱ��
	virtual connect_client* create_connect();

private:
	polarssl_conf* ssl_conf_;
};

class ACL_CPP_API http_guard : public connect_guard
{
public:
	http_guard(http_request_pool& pool);
	~http_guard(void);
};

} // namespace acl
