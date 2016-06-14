#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include <map>
#include "acl_cpp/stdlib/dbuf_pool.hpp"
#include "acl_cpp/stdlib/string.hpp"

namespace acl {

// �����洢����ֵ�Ļ����������壬����Ҫ��Ϊ�˼�������ֵ
// ����Ϊ�����Ƶ����ζ����ӵĽṹ����
typedef enum
{
	TODO_NUL,
	TODO_SET,
	TODO_DEL
} todo_t;

class ACL_CPP_API session_string : public string
{
public:
	session_string(size_t n = 64) : string(n), todo_(TODO_NUL) {}
	session_string(const session_string& ss)
		: string(ss)
	{
		todo_ = ss.todo_;
	}
	session_string(const string& s) : string(s), todo_(TODO_NUL) {}
	session_string(const char* s) : string(s), todo_(TODO_NUL) {}
	~session_string() {}
	todo_t todo_;
};

/**
 * session �࣬����ʹ�� memcached �洢 session ����
 */
class ACL_CPP_API session : public dbuf_obj
{
public:
	/**
	 * �����캯���Ĳ��� sid �ǿ�ʱ����� session ����ʹ�ø�
	 * sid�������ڲ����Զ�����һ�� sid���û�Ӧ��ͨ�� get_sid()
	 * �������Զ����ɵ� sid �Ա���ÿ�β�ѯ�� sid ��Ӧ������
	 * @param ttl {time_t} ָ�� session ����������(��)
	 * @param sid {const char*} �ǿ�ʱ���� session �� sid ʹ
	 *  ��ֵ�������ڲ������һ������� session sid���������
	 *  sid ����ͨ������ get_sid() ��ã���Ȼ��ʹ�ù����У��û�
	 *  Ҳ����ͨ�� set_sid() �޸ı������ session sid��
	 *  ���⣬����� sid Ϊ�գ�������û������ĳ�� sid ��Ӧ��
	 *  ���ݣ����û������ȵ��� set_sid()
	 */
	session(time_t ttl = 0, const char* sid = NULL);
	virtual ~session(void);
	
	/**
	 * �����ڲ�״̬�������һЩ��ʱ����
	 */
	void reset(void);

	/**
	 * ��ñ� session �����Ψһ ID ��ʶ
	 * @return {const char*} �ǿ�
	 */
	virtual const char* get_sid(void) const
	{
		return sid_.c_str();
	}

	/**
	 * ���ñ� session �����Ψһ ID ��ʶ
	 * @param sid {const char*} �ǿ�
	 * ע�����ñ������󣬻��Զ����֮ǰ���м仺������
	 */
	void set_sid(const char* sid);

	/**
	 * ������ session ��� set/set_ttl ʱ��������һ������ delay Ϊ true��
	 * �����ͨ�����ñ������������������и���
	 * @return {bool} ���ݸ����Ƿ�ɹ�
	 */
	virtual bool flush();

	/**
	 * �� session ������µ��ַ������ԣ�ͬʱ���ø�
	 * session �Ĺ���ʱ����(��)
	 * @param name {const char*} session �����ǿ�
	 * @param value {const char*} session ֵ���ǿ�
	 * @return {bool} ���� false ��ʾ����
	 */
	virtual bool set(const char* name, const char* value);

	/**
	 * �� session ������µ����Զ������ø� session �Ĺ���ʱ����(��)��
	 * @param name {const char*} session ���������ǿ�
	 * @param value {const char*} session ����ֵ���ǿ�
	 * @param len {size_t} value ֵ����
	 * @return {bool} ���� false ��ʾ����
	 */
	virtual bool set(const char* name, const void* value, size_t len);

	/**
	 * �ӳ��� session ������µ����Զ������ø� session �Ĺ���ʱ����(��)��
	 * ���û������� session::flush ���ٽ������ݸ��£� ����������ߴ���Ч��
	 * @param name {const char*} session ���������ǿ�
	 * @param value {const char*} session ����ֵ���ǿ�
	 * @param len {size_t} value ֵ����
	 * @return {bool} ���� false ��ʾ����
	 */
	virtual bool set_delay(const char* name, const void* value, size_t len);
	
	/**
	 * �� session ��ȡ���ַ�����������ֵ
	 * @param name {const char*} session ���������ǿ�
	 * @return {const char*} session ����ֵ�����ص�ָ���ַ��Զ�ǿգ��û�
	 *  ����ͨ���жϷ��ص��Ƿ��ǿմ�(��: "\0")���жϳ���򲻴���
	 *  ע���ú������طǿ����ݺ��û�Ӧ�����̱����˷���ֵ����Ϊ�´�
	 *      �������������ÿ��ܻ��������ʱ��������
	 */
	const char* get(const char* name);

	/**
	 * �� session ��ȡ�ö������������͵�����ֵ
	 * @param name {const char*} session ���������ǿ�
	 * @return {const session_string*} session ����ֵ�����ؿ�ʱ
	 *  ��ʾ����򲻴���
	 *  ע���ú������طǿ����ݺ��û�Ӧ�����̱����˷���ֵ����Ϊ�´�
	 *      �������������ÿ��ܻ��������ʱ��������
	 */
	virtual const session_string* get_buf(const char* name);

	/**
	 * �� session ��ɾ��ָ������ֵ�������еı�����ɾ��
	 * ʱ�Ὣ��������� memcached ��ɾ��
	 * @param name {const char*} session ���������ǿ�
	 * @return {bool} true ��ʾ�ɹ�(�����������)��false ��ʾɾ��ʧ��
	 *  ע���������ӳٷ�ʽɾ��ĳ������ʱ�����ӳٷ��͸���ָ���˵�
	 *  ��������������û������� session::flush ���ٽ������ݸ��£���
	 *  ��������ߴ���Ч�ʣ����������̸�������
	 */
	virtual bool del_delay(const char* name);
	virtual bool del(const char* name);

	/**
	 * �������� session �ڻ���������ϵĻ���ʱ��
	 * @param ttl {time_t} ��������(��)
	 * @param delay {bool} ��Ϊ true ʱ�����ӳٷ��͸���ָ���˵�
	 *  ��������������û������� session::flush ���ٽ������ݸ��£���
	 *  ��������ߴ���Ч�ʣ���Ϊ false ʱ�������̸�������
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool set_ttl(time_t ttl, bool delay);

	/**
	 * ��ñ� session �����м�¼�� session �������ڣ���ֵ�п���
	 * �������洢�ڻ����������ʱ�䲻һ�£���Ϊ�п���������ʵ��
	 * ���������� session �ڻ���������ϵ���������
	 * @return {time_t}
	 */
	time_t get_ttl(void) const
	{
		return ttl_;
	}

	/**
	 * ʹ session �ӷ���˵Ļ�����ɾ����ʹ session ʧЧ
	 * @return {bool} �Ƿ�ʹ session ʧЧ
	 */
	virtual bool remove(void) = 0;

	/**
	 * �Ӻ�˻����л�ö�Ӧ sid �����Զ��󼯺�
	 * @param attrs {std::map<string, session_string>&}
	 * @return {bool}
	 */
	virtual bool get_attrs(std::map<string, session_string>& attrs) = 0;

	/**
	 * �Ӻ�˻����л�ö�Ӧ sid ��ָ�����Լ���
	 * @param names {const std::vector<string>&} ����������
	 * @param values {std::vector<session_string>&} �洢��Ӧ������ֵ�����
	 * @return {bool} �����Ƿ�ɹ�
	 */
	virtual bool get_attrs(const std::vector<string>& names,
		std::vector<session_string>& values);

	/**
	 * ���˻���д���Ӧ sid �����Զ��󼯺�
	 * @param attrs {std::map<string, session_string>&}
	 * @return {bool}
	 */
	virtual bool set_attrs(const std::map<string, session_string>& attrs) = 0;

protected:
	// ���ö�Ӧ sid ���ݵĹ���ʱ��
	virtual bool set_timeout(time_t ttl) = 0;

protected:
	// �� session �������л�
	static void serialize(const std::map<string, session_string>& attrs,
		string& out);

	// �� session ���ݷ����л�
	static void deserialize(string& buf,
		std::map<string, session_string>& attrs);

	// ��� session ���Լ���
	static void attrs_clear(std::map<string, session_string>& attrs);

protected:
	session_string sid_;
	time_t ttl_;

	// �ñ�����Ҫ���� set_ttl �����У�����Ʋ�� sid_ ֻ���²�����
	// �һ�û���ں�� cache ����˴洢���� set_ttl �����������º��
	// �� cache ������
	bool sid_saved_;
	bool dirty_;
	std::map<string, session_string> attrs_;
	std::map<string, session_string> attrs_cache_;
};

} // namespace acl
