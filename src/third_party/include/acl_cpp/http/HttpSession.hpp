#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/stdlib/dbuf_pool.hpp"
#include "acl_cpp/session/session.hpp"
#include <map>

namespace acl {

class session;

/**
 * ����� HttpSession �࣬Ŀǰ��������ݴ洢ֻ��֧�ִ��� memcached ��
 */
class ACL_CPP_API HttpSession : public dbuf_obj
{
public:
	/**
	 * ���캯��
	 * @param session {session&} �������
	 */
	HttpSession(session& session);
	virtual ~HttpSession(void);

	/**
	 * ��ÿͻ����ڷ���˴洢�� session ���ַ�������ֵ
	 * @param name {const char*} session ���������ǿ�
	 * @return {const char*} session ����ֵ�����ص�ַ��Զ�ǿ�ָ�룬�û�
	 *  ����ͨ���жϷ��ص�ַ�Ƿ�Ϊ�մ�("\0")���ж��Ƿ���ڻ����
	 *  ע���ú������طǿ����ݺ��û�Ӧ�����̱����˷���ֵ����Ϊ�´�
	 *      �������������ÿ��ܻ��������ʱ��������
	 */
	virtual const char* getAttribute(const char* name) const;

	/**
	 * ��ÿͻ����ڷ���˴洢�� session �Ķ���������ֵ
	 * @param name {const char*} session ���������ǿ�
	 * @param size {size_t*} �ò����ǿ�������ֵ�ǿ�ʱ����ָ���ַ
	 *  �洢��������ֵ�Ĵ�С
	 * @return {const void*} session ����ֵ��Ϊ��ָ��ʱ˵��������
	 *  ���ڲ���ѯʧ��
	 *  ע���ú������طǿ����ݺ��û�Ӧ�����̱����˷���ֵ����Ϊ�´�
	 *      �������������ÿ��ܻ��������ʱ��������
	 */
	virtual const void* getAttribute(const char* name, size_t* size) const;

	/**
	 * �ӷ���˻�ö�Ӧ�ͻ��˵����лỰ���Զ����������Լ��������˵Ľ�������
	 * @param attrs {std::map<string, session_string>&}
	 * @return {bool} �Ƿ�ɹ�
	 */
	virtual bool getAttributes(std::map<string, session_string>& attrs) const;

	/**
	 * �ӷ���˻�ö�Ӧ�ͻ��˵���Ӧ���Լ���
	 * @param names {const std::vector<string>&} ����������
	 * @param values {std::vector<session_string>&} �洢��Ӧ������ֵ�����
	 * @return {bool} �Ƿ�ɹ�
	 */
	virtual bool getAttributes(const std::vector<string>& names,
		std::vector<session_string>& values) const;

	/**
	 * �����÷�������� session ���ַ�������ֵ
	 * @param name {const char*} session ���������ǿ�
	 * @param value {const char*} session ����ֵ���ǿ�
	 * @return {bool} ���� false ˵������ʧ��
	 */
	virtual bool setAttribute(const char* name, const char* value);

	/**
	 * �����÷�������� session �Ķ���������ֵ
	 * @param name {const char*} session ���������ǿ�
	 * @param value {const void*} session ����ֵ���ǿ�
	 * @param len {size_t} value ���ݳ���
	 * @return {bool} ���� false ˵������ʧ��
	 */
	virtual bool setAttribute(const char* name, const void* value, size_t len);

	/**
	 * �ڷ�������� session ���Լ��ϣ��������Լ������˵Ľ�������
	 * @param attrs {const std::map<string, session_string>&} ���Լ��϶���
	 * @return {bool} �����Ƿ�ɹ�
	 */
	virtual bool setAttributes(const std::map<string, session_string>& attrs);

	/**
	 * ɾ���ͻ��� session �е�ĳ������ֵ
	 * @param name {const char*} session ���������ǿ�
	 * @return {bool} ɾ���Ƿ�ɹ�
	 */
	virtual bool removeAttribute(const char* name);

	/**
	 * ���� session �ڻ���������ϵ���������
	 * @param ttl {time_t} ��������(��)
	 * @return {bool} �Ƿ�ɹ�
	 */
	virtual bool setMaxAge(time_t ttl);

	/**
	 * ʹ session �ӷ���˵Ļ�����ɾ����ʹ session ʧЧ
	 * @return {bool} �Ƿ�ʹ session ʧЧ
	 */
	virtual bool invalidate(void);

	/**
	 * ����������� session ID ��ʶ
	 * @return {const char*} ��Զ������ '\0' ��β�ķǿ�ָ�룬�ɸ��ݷ���
	 *  ֵ�Ƿ�Ϊ�մ�("\0")���ж� sid �Ƿ����
	 */
	const char* getSid(void) const;

protected:
	session& session_;
};

} // namespace acl
