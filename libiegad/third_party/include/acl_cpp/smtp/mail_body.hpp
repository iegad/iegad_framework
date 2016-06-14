#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include <vector>
#include "acl_cpp/stdlib/string.hpp"
#include "acl_cpp/http/http_ctype.hpp"

namespace acl {

class mime_code;
class mail_attach;

/**
 * �ʼ����Ĺ�����
 */
class ACL_CPP_API mail_body
{
public:
	/**
	 * ���캯��
	 * @param charset {const char*} ���ĵ��ַ���
	 * @param encoding {const char*} ���ĵı����ʽ
	 */
	mail_body(const char* charset = "utf-8",
		const char* encoding = "base64");
	~mail_body();

	/**
	 * �����������������
	 * @return {const string&}
	 */
	const string& get_content_type() const
	{
		return content_type_;
	}

	/**
	 * ��������������Ͷ���
	 * @return {const http_ctype&}
	 */
	const http_ctype& get_ctype() const
	{
		return ctype_;
	}

	/**
	 * �����ʼ�����Ϊ TEXT/HTML ��ʽ
	 * @param html {const char*} HTML ����
	 * @param len {size_t} html ���ݳ���(��Ȼ html ���ַ�����ʽ�����ṩ
	 *  ���ݳ��������ڵ��ø�����Ч���ڲ���������ͨ�� strlen ���㳤��)
	 * @return {mail_body&}
	 */
	mail_body& set_html(const char* html, size_t len);

	/**
	 * �����ʼ�����Ϊ TEXT/PLAIN ��ʽ
	 * @param plain {const char*} TEXT ����
	 * @param len {size_t} plain ���ݳ���(��Ȼ plain ���ı���ʽ�����ṩ
	 *  ���ݳ��������ڵ��ø�����Ч���ڲ���������ͨ�� strlen ���㳤��)
	 * @return {mail_body&}
	 */
	mail_body& set_plain(const char* plain, size_t len);

	/**
	 * ���ʼ�����Ϊ multipart/alternative ��ʽʱ���ô˺���������Ӧ���͵�
	 * ��������
	 * @param html {const char*} �����е� HTML ����(�ǿ�)
	 * @param hlen {size_t} html ���ݳ���(>0)
	 * @param plain {const char*} �����е� TEXT ����(�ǿ�)
	 * @param plen {size_t} plain ���ݳ���(>0)
	 * @return {mail_body&}
	 */
	mail_body& set_alternative(const char* html, size_t hlen,
		const char* plain, size_t plen);

	/**
	 * ���ʼ���������Ϊ multipart/relative ��ʽʱ���ô˺���������������
	 * @param html {const char*} �����е� HTML ����(�ǿ�)
	 * @param hlen {size_t} html ���ݳ���(>0)
	 * @param plain {const char*} �����е� plain ����(�ǿ�)
	 * @param plen {size_t} plain ���ݳ���(>0)
	 * @param attachments {const std::vector<mail_attach*>&} ���
	 *  �� html �е� cid ��ص�ͼƬ�ȸ�������
	 * @return {mail_body&}
	 */
	mail_body& set_relative(const char* html, size_t hlen,
		const char* plain, size_t plen,
		const std::vector<mail_attach*>& attachments);

	/**
	 * ��� set_html �������õ� html/plain ����
	 * @param len {size_t} ������ݳ��Ƚ��
	 * @return {const char*}
	 */
	const char* get_html(size_t& len) const
	{
		len = hlen_;
		return html_;
	}

	/**
	 * ��� set_plain �������õ� plain/plain ����
	 * @param len {size_t} ������ݳ��Ƚ��
	 * @return {const char*}
	 */
	const char* get_plain(size_t& len) const
	{
		len = plen_;
		return plain_;
	}

	/**
	 * ��� set_attachments �������õĸ�������
	 * @return {const std::vector<mail_attach*>*}
	 */
	const std::vector<mail_attach*>* get_attachments() const
	{
		return attachments_;
	}

	/**
	 * �����ʼ����Ĳ������׷���ڸ������������
	 * @param out {ostream&} ���������
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool save_to(ostream& out) const;

	/**
	 * �����ʼ����Ĳ������׷���ڸ����Ļ�������
	 * @param out {string&} �洢���
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool save_to(string& out) const;

	/**
	 * text/html ��ʽ���ʼ����Ĺ�����̣��������׷���ڸ����Ļ�������
	 * @param in {const char*} ����� html ��ʽ����
	 * @param len {size_t} in �����ݳ���
	 * @param out {string&} ������׷�ӷ�ʽ�洢���
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool save_html(const char* in, size_t len, string& out) const;

	/**
	 * text/plain ��ʽ���ʼ����Ĺ�����̣��������׷���ڸ����Ļ�������
	 * @param in {const char*} ����� plain ��ʽ����
	 * @param len {size_t} in �����ݳ���
	 * @param out {string&} ������׷�ӷ�ʽ�洢���
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool save_plain(const char* in, size_t len, string& out) const;

	/**
	 * multipart/relative ��ʽ���ʼ����Ĺ�����̣��������׷���ڸ����Ļ�������
	 * @param html {const char*} ����� html ��ʽ����
	 * @param hlen {size_t} html �����ݳ���
	 * @param plain {const char*} �����е� TEXT ����(�ǿ�)
	 * @param plen {size_t} plain ���ݳ���(>0)
	 * @param attachments {const std::vector<mail_attach*>&} ���
	 *  �� html �е� cid ��ص�ͼƬ�ȸ�������
	 * @param out {string&} ������׷�ӷ�ʽ�洢���
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool save_relative(const char* html, size_t hlen,
		const char* plain, size_t plen,
		const std::vector<mail_attach*>& attachments,
		string& out) const;

	/**
	 * multipart/alternative ��ʽ���ʼ����Ĺ�����̣��������׷���ڸ����Ļ�������
	 * @param html {const char*} ����� html ��ʽ����
	 * @param hlen {size_t} html �����ݳ���
	 * @param plain {const char*} �����е� TEXT ����(�ǿ�)
	 * @param plen {size_t} plain ���ݳ���(>0)
	 * @param out {string&} ������׷�ӷ�ʽ�洢���
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool save_alternative(const char* html, size_t hlen,
		const char* plain, size_t plen, string& out) const;

private:
	string  charset_;
	string  content_type_;
	string  transfer_encoding_;
	mime_code* coder_;
	string  boundary_;
	http_ctype ctype_;
	int     mime_stype_;

	const char* html_;
	size_t hlen_;
	const char* plain_;
	size_t plen_;
	const std::vector<mail_attach*>* attachments_;

	bool build(const char* in, size_t len, const char* content_type,
		const char* charset, mime_code& coder, string& out) const;
	bool build_html(const char* in, size_t len,
		const char* charset, string& out) const;
	bool build_plain(const char* in, size_t len,
		const char* charset, string& out) const;

	void set_content_type(const char* content_type);
};

} // namespace acl
