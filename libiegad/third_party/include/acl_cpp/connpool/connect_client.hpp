#pragma once
#include "acl_cpp/acl_cpp_define.hpp"

namespace acl
{

class connect_pool;

class ACL_CPP_API connect_client
{
public:
	connect_client() : when_(0), pool_(NULL) {}
	virtual ~connect_client() {}

	/**
	 * ��ø����Ӷ������һ�α�ʹ�õ�ʱ���
	 * @return {time_t}
	 */
	time_t get_when()
	{
		return when_;
	}

	/**
	 * ���ø����Ӷ���ǰ��ʹ�õ�ʱ���
	 */
	void set_when(time_t when)
	{
		when_ = when;
	}

	/**
	 * ���麯�����������ʵ�ִ˺����������ӷ�����
	 * @return {bool} �Ƿ����ӳɹ�
	 */
	virtual bool open() = 0;

	/**
	 * ������ӳض������ã��� connect_pool �ڲ�����
	 * ���Ӷ������� set_pool �������ӳض�����
	 * @return {connect_pool*}
	 */
	connect_pool* get_pool() const
	{
		return pool_;
	}

public:
	/**
	 * �麯�����ú��������������ӳ�ʱʱ�估���� IO ��ʱʱ�䣬����������ظ��麯����
	 * �Ա��������ڲ��Ķ���ʱʱ��
	 * @param conn_timeout {int} �������ӳ�ʱʱ��(��)
	 * @param rw_timeout {int} ���� IO ��ʱʱ��(��)
	 */
	virtual void set_timeout(int conn_timeout, int rw_timeout)
	{
		conn_timeout_ = conn_timeout;
		rw_timeout_ = rw_timeout;
	}

protected:
	int   conn_timeout_;
	int   rw_timeout_;

	friend class connect_pool;

	time_t when_;
	connect_pool* pool_;

	void set_pool(connect_pool* pool)
	{
		pool_ = pool;
	}
};

} // namespace acl
