#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/stdlib/string.hpp"
#include <vector>

namespace acl {

class dbuf_pool;
struct rfc822_addr;
class mail_attach;
class mail_body;
class ofstream;

/**
 * �ʼ����ݹ����࣬�����������һ���������ʼ���ͬʱ�����ڹ��� SMTP ���͹���
 * ���ʼ��ŷ���Ϣ
 */
class ACL_CPP_API mail_message
{
public:
	/**
	 * ���캯��
	 * @param charset {const char*} �ַ���
	 */
	mail_message(const char* charset = "utf-8");
	~mail_message();

	/**
	 * ���� SMTP ���͹��̵������֤��Ϣ
	 * @param user {const char*} �����˺�
	 * @param pass {const char*} ��������
	 * @return {mail_message&}
	 */
	mail_message& set_auth(const char* user, const char* pass);

	/**
	 * �����ʼ��ķ��Ͷ����䣬���ֶο����� SMTP ���͹��̵� MAIL FROM ���
	 * ͬʱ�ֿ���Ϊ�ʼ�ͷ�е� From �ֶ�ֵ
	 * @param from {const char*} �������ʼ���ַ
	 * @param name {const char*} ����������
	 * @return {mail_message&}
	 */
	mail_message& set_from(const char* from, const char* name = NULL);

	mail_message& set_sender(const char* sender, const char* name = NULL);

	/**
	 * �����ʼ�ͷ�е� Reply-To �ֶ�ֵ
	 * @param reply_to {const char*} Reply-To �����ֶ�ֵ
	 * @param name {const char*} Reply-To ��Ӧ����Ա����
	 * @return {mail_message&}
	 */
	mail_message& set_reply_to(const char* reply_to, const char* name = NULL);

	/**
	 * �����ʼ�ͷ�е� Return-Path �ֶ�ֵ
	 * @param return_path {const char*} Return-Path �����ֶ�ֵ
	 * @return {mail_message&}
	 */
	mail_message& set_return_path(const char* return_path);

	/**
	 * �����ʼ�ͷ�е� Delivered-To �ֶ�ֵ
	 * @param delivered_to {const char*} Delivered-To �����ֶ�ֵ
	 * @return {mail_message&}
	 */
	mail_message& set_delivered_to(const char* delivered_to);

	/**
	 * ����ռ��˵�ַ���õ�ַ���������ŷ��У����������ʼ�ͷ��
	 * @param recipients {const char*} �ռ��˼��ϣ����� RFC822 ��ʽ
	 * @return {mail_message&}
	 */
	mail_message& add_recipients(const char* recipients);

	/**
	 * �����ʼ�ͷ�е� To �ֶ�ֵ��ͬʱ���ռ��˵�ַ���ϱ������ŷ�����Ϊ�ռ���
	 * @param to {const char*} �ռ��������ַ���ϣ����� RFC822 ��ʽ
	 * @return {mail_message&}
	 */
	mail_message& add_to(const char* to);

	/**
	 * �����ʼ�ͷ�е� Cc �ֶ�ֵ��ͬʱ���ռ��˵�ַ���ϱ������ŷ�����Ϊ�ռ���
	 * @param cc {const char*} �ռ��������ַ���ϣ����� RFC822 ��ʽ
	 * @return {mail_message&}
	 */
	mail_message& add_cc(const char* cc);

	/**
	 * �����ʼ����͵İ��͵�ַ���ϣ��õ�ַ���ϲ���������ʼ�ͷ��
	 * @param bcc {const char*} ���������ַ���ϣ����� RFC822 ��ʽ
	 * @return {mail_message&}
	 */
	mail_message& add_bcc(const char* bcc);

	/**
	 * �����ʼ�ͷ�е����⣬�����⽫���� rfc2047 �����Ҳ����๹�캯��
	 * ���õ��ַ���
	 * @param subject {const char*} �ʼ�ͷ�����ֶ�ֵ
	 * @return {mail_message&}
	 */
	mail_message& set_subject(const char* subject);

	/**
	 * �û����Ե��ô˺�������ʼ�ͷ�е�ͷ����չ�ֶ�ֵ
	 * @param name {const char*} �ֶ���
	 * @param value {const char*} �ֶ�ֵ
	 * @return {mail_message&}
	 */
	mail_message& add_header(const char* name, const char* value);

	/**
	 * �����ʼ������Ķ���
	 * @param body {const mail_body&} �ʼ����Ķ���
	 * @return {mail_message&}
	 */
	mail_message& set_body(const mail_body& body);

	/**
	 * ��һ���ʼ����һ������
	 * @param filepath {const char*} ����ȫ·�����ǿգ�
	 * @param content_type {const char*} �������ͣ��ǿգ�
	 * @return {mail_message&}
	 */
	mail_message& add_attachment(const char* filepath,
		const char* content_type);

	/**
	 * ����һ���������ʼ��������ʼ����ݴ洢�ڸ��������ļ��У�������ļ�
	 * ���������Ȼ���գ�����ᴴ���µ��ļ�
	 * @param filepath {const char*} Ŀ���ļ�
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool save_to(const char* filepath);

	/**
	 * ���Ե������ñ��������������ʼ�ͷ����
	 * @param out {string&} �������ʼ�ͷ���ݽ�׷���ڸû�������
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool build_header(string& out);

	/**
	 * ������������ʼ��ڴ����ϵ�ȫ·�����ú��������ڵ��� save_to �ɹ������
	 * @return {const char*}
	 */
	const char* get_email() const
	{
		return filepath_;
	}

	/**
	 * ������� SMTP �����֤ʱ�������˺�
	 * @return {const char*}
	 */
	const char* get_auth_user() const
	{
		return auth_user_;
	}

	/**
	 * ������� SMTP �����֤ʱ�������˺�����
	 * @return {const char*}
	 */
	const char* get_auth_pass() const
	{
		return auth_pass_;
	}

	/**
	 * ����� set_from ���õ������ַ����
	 * @return {const rfc822_addr*}
	 */
	const rfc822_addr* get_from() const
	{
		return from_;
	}

	/**
	 * ����� set_sender ���õ������ַ����
	 * @return {const rfc822_addr*}
	 */
	const rfc822_addr* get_sender() const
	{
		return sender_;
	}

	/**
	 * ����� set_reply_to ���õ������ַ����
	 * @return {const rfc822_addr*}
	 */
	const rfc822_addr* get_reply_to() const
	{
		return reply_to_;
	}

	/**
	 * ����� set_return_path ���õ������ַ����
	 * @return {const rfc822_addr*}
	 */
	const rfc822_addr* get_return_path() const
	{
		return return_path_;
	}

	/**
	 * ����� set_delivered_to ���õ������ַ����
	 * @return {const rfc822_addr*}
	 */
	const rfc822_addr* get_delivered_to() const
	{
		return delivered_to_;
	}

	const std::vector<rfc822_addr*>& get_to() const
	{
		return to_list_;
	}

	/**
	 * ����� set_cc ���õ������ַ���󼯺�
	 * @return {const std::vector<rfc822_addr*>&}
	 */
	const std::vector<rfc822_addr*>& get_cc() const
	{
		return cc_list_;
	}

	/**
	 * ����� set_bcc ���õ������ַ���󼯺�
	 * @return {const std::vector<rfc822_addr*>&}
	 */
	const std::vector<rfc822_addr*>& get_bcc() const
	{
		return bcc_list_;
	}

	/**
	 * ��������ʼ������ߵĵ�ַ����
	 * @return {const std::vector<rfc822_addr*>&}
	 */
	const std::vector<rfc822_addr*>& get_recipients() const
	{
		return recipients_;
	}

	/**
	 * ����û����õ��ʼ�ͷ��չ�ֶ�ֵ
	 * @param name {const char*} �ֶ���
	 * @return {const char*}
	 */
	const char* get_header_value(const char* name) const;

	/**
	 * Ϊ MIME ���ݴ���Ψһ�ķָ���
	 * @param id {const char*} ��������д�� ID ��ʶ
	 * @param out {string&} �洢���
	 */
	static void create_boundary(const char* id, string& out);

private:
	dbuf_pool* dbuf_;
	char* charset_;
	char* transfer_encoding_;

	char* auth_user_;
	char* auth_pass_;
	rfc822_addr* from_;
	rfc822_addr* sender_;
	rfc822_addr* reply_to_;
	rfc822_addr* return_path_;
	rfc822_addr* delivered_to_;
	std::vector<rfc822_addr*> to_list_;
	std::vector<rfc822_addr*> cc_list_;
	std::vector<rfc822_addr*> bcc_list_;
	std::vector<rfc822_addr*> recipients_;
	char* subject_;
	std::vector<std::pair<char*, char*> > headers_;
	std::vector<mail_attach*> attachments_;
	const mail_body* body_;
	size_t body_len_;
	char* filepath_;

	void add_addrs(const char* in, std::vector<rfc822_addr*>& out);
	bool append_addr(const rfc822_addr& addr, string& out);
	bool append_addr(const char* name, const rfc822_addr& addr,
		string& out);
	bool append_addrs(const char* name,
		const std::vector<rfc822_addr*>& addrs, string& out);
	bool append_message_id(string& out);
	bool append_subject(const char* subject, string& out);
	bool append_date(string& out);
	bool append_header(ofstream& fp);
	bool append_multipart(ofstream& fp);
};

} // namespace acl
