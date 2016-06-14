#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/db/db_handle.hpp"

typedef struct st_mysql MYSQL;

namespace acl {

class mysql_conf;

class ACL_CPP_API db_mysql : public db_handle
{
public:
	/**
	 * ���캯����ʽһ
	 * @param dbaddr {const char*} ���ݿ������ַ������Ϊ TCP �׽ӿڻ��� UNIX
	 *  ƽ̨�µ����׽ӿڣ���ʽ�磺127.0.0.1:3306���� /tmp/mysql.sock
	 * @param dbname {const char*} ���ݿ����ƣ��� NULL
	 * @param dbuser {const char*} �������ݿ�ʱ���û���
	 * @param dbpass {const char*} �������ݿ�ʱ���û�����
	 * @param dbflags {unsigned long} ���� MYSQL ʱ�ı�־λ
	 * @param auto_commit {bool} �������ݿ�����޸�ʱ�Ƿ��Զ��ύ����
	 * @param conn_timeout {int} �������ݿ�ĳ�ʱʱ�䣨�룩
	 * @param rw_timeout {int} �������ݿ����ʱ�ĳ�ʱʱ�䣨�룩
	 * @param charset {const char*} �������ݿ�ʱ�ı����ַ�����gbk, utf8, ...��
	 */
	db_mysql(const char* dbaddr, const char* dbname,
		const char* dbuser, const char* dbpass,
		unsigned long dbflags = 0, bool auto_commit = true,
		int conn_timeout = 60, int rw_timeout = 60,
		const char* charset = "utf8");

	/**
	 * ���캯����ʽ����ʹ�ò��������������й���
	 * @param conf {const mysql_conf&} mysql ���ݿ��������������
	 */
	db_mysql(const mysql_conf& conf);
	~db_mysql(void);

	/**
	 * ��� mysql �ͻ��˿�İ汾��
	 * @return {unsigned long}
	 */
	unsigned long mysql_libversion() const;

	/**
	 * ��� mysql �ͻ��˿����Ϣ
	 * @return {const char*}
	 */
	const char* mysql_client_info() const;

	/**
	 * ֱ�ӻ�� mysql �����Ӿ����������� NULL ���ʾ mysql ��û�д�
	 * �����ʱ�ڲ��Զ��ر��� mysql ����
	 * @return {MYSQL*}
	 */
	MYSQL* get_conn() const
	{
		return conn_;
	}

	/********************************************************************/
	/*         ����Ϊ���� db_handle ����ӿ�                            */
	/********************************************************************/

	/**
	 * �������ݿ����������
	 * @return {const char*}
	 */
	const char* dbtype() const;

	/**
	 * ����ϴ����ݿ�����ĳ�������
	 * @return {int}
	 */
	int get_errno() const;

	/**
	 * ����ϴ����ݿ�����ĳ��������
	 * @return {const char*}
	 */
	const char* get_error() const;

	/**
	 * ���� db_handle �Ĵ���ӿ�
	 * @param charset {const char*} �����ݿ�����ʱ���õ��ַ���������
	 *  �����ǿ�ʱ���Ḳ�ǹ��캯���д�����ַ���
	 * @return {bool} ���Ƿ�ɹ�
	 */
	bool dbopen(const char* charset = NULL);

	/**
	 * ���� db_handle �Ĵ���ӿڣ����ݿ��Ƿ��Ѿ�����
	 * @return {bool} ���� true �������ݿ��Ѿ�����
	 */
	bool is_opened() const;

	/**
	 * ���� db_handle �Ĵ���ӿ�
	 * @return {bool} �ر��Ƿ�ɹ�
	 */
	bool close(void);

	/**
	 * ���� db_handle �Ĵ���ӿڣ��������ʵ�ִ˽ӿ������ж����ݱ��Ƿ����
	 * @return {bool} �Ƿ����
	 */
	bool tbl_exists(const char* tbl_name);

	/**
	 * ���� db_handle �Ĵ���ӿ�
	 * @param sql {const char*} ��׼�� SELECT SQL ��䣬�ǿգ�����һ��
	 *  ��Ҫע��� SQL �����뾭��ת�崦���Է�ֹ SQL ע�빥��
	 * @return {bool} ִ���Ƿ�ɹ�
	 */
	bool sql_select(const char* sql);

	/**
	 * ���� db_handle �Ĵ���ӿ�
	 * @param sql {const char*} ��׼�� INSERT/UPDATE/DELETE SQL ��䣬
	 *  ����һ����Ҫע��� SQL �����뾭��ת�崦���Է�ֹ SQL ע�빥��
	 * @return {bool} ִ���Ƿ�ɹ�
	 */
	bool sql_update(const char* sql);

	/**
	 * ���� db_handle �Ĵ���ӿڣ��ϴ� sql ����Ӱ��ļ�¼����
	 * @return {int} Ӱ���������-1 ��ʾ����
	 */
	int affect_count() const;

	/**
	 * ���� db_handle ���麯����������ʾ����Ŀ�ʼ��ע����Ҫʹ������ʽ��
	 * ����Ҫ�� db_mysql �Ĺ��캯���д���Ĳ��� auto_commit Ϊ false
	 * @return {bool}
	 */
	bool begin_transaction();

	/**
	 * ���� db_handle ���麯����������ʾ����Ľ���
	 * @return {bool}
	 */
	bool commit();

private:
	char* dbaddr_;  // ���ݿ������ַ
	char* dbname_;  // ���ݿ���
	char* dbuser_;  // ���ݿ��˺�
	char* dbpass_;  // ���ݿ��˺�����
	string charset_; // �������ݿ���õ��ַ���

	unsigned long dbflags_;
	int   conn_timeout_;
	int   rw_timeout_;
	bool  auto_commit_;
	MYSQL* conn_;

	bool sane_mysql_query(const char* sql);
	void sane_mysql_init(const char* dbaddr, const char* dbname,
		const char* dbuser, const char* dbpass,
		unsigned long dbflags, bool auto_commit,
		int conn_timeout, int rw_timeout,
		const char* charset);
};

} // namespace acl
