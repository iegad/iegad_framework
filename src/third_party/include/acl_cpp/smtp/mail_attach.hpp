#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/stdlib/string.hpp"

namespace acl {

class mime_code;
class ostream;

/**
 * ׫д�ʼ�ʱ���������ڴ������ʼ�������صĹ���
 */
class ACL_CPP_API mail_attach
{
public:
	/**
	 * ��һ����ͨ�ļ�������ʼ�ʱ�Ĺ��캯��
	 * @param filepath {const char*} �����ļ��洢·�������ļ�����
	 * @param content_type {const char*} �����ļ�����
	 * @param charset {const char*} ��Ϊ���ļ����˲����������ı����ַ���
	 */
	mail_attach(const char* filepath, const char* content_type,
		const char* charset);
	~mail_attach();

	/**
	 * ���ø������ļ������ڲ����Զ����ļ����� rfc2047 ��ʽ���б���
	 * @param name {const char*} �ǿ��ַ���
	 * @param charset {const char*} �ò���ָ���ַ��������� NULL ʱ������
	 *  ���Զ�ʹ�� rfc2047 ��ʽ���ļ������б��룬�����ڲ�ֱ�Ӵ洢��������
	 * @return {mail_attach&}
	 */
	mail_attach& set_filename(const char* name, const char* charset = NULL);

	/**
	 * ���ʼ��е�������Ϊ multipart/relative ����ʱ�����ô˺����������е�
	 * html ������ cid ��ʶ��
	 * @param id {const char*} cid ��ʶ��
	 * @return {mail_attach&}
	 */
	mail_attach& set_content_id(const char* id);

	/**
	 * ��ù��캯������ĸ����ļ�·��
	 * @return {const char*}
	 */
	const char* get_filepath() const
	{
		return filepath_.c_str();
	}

	/**
	 * ��ø������ļ������־� rfc2047 ���������
	 * @return {const char*}
	 */
	const char* get_filename() const
	{
		return filename_.c_str();
	}

	/**
	 * ��ù��캯��������ļ�����
	 * @return {const char*}
	 */
	const char* get_content_type() const
	{
		return ctype_.c_str();
	}

	/**
	 * ����� set_content_id ���õĸø����� cid ��ʶ��
	 * @return {const char*}
	 */
	const char* get_content_id() const
	{
		return cid_.c_str();
	}

	/**
	 * ���������ݲ��ô���ı��������б��������ڴ滺����
	 * @param coder {mime_code*} ��������base64/qp�ȣ�
	 * @param out {string&} �洢��������� append ��ʽ
	 * @return {bool} ��������Ƿ�ɹ�
	 */
	bool save_to(mime_code* coder, string& out);

	/**
	 * ���������ݲ��ô���ı��������б��������������
	 * @param coder {mime_code*} ��������base64/qp�ȣ�
	 * @param out {out&} �洢���
	 * @return {bool} ��������Ƿ�ɹ�
	 */
	bool save_to(mime_code* coder, ostream& out);

	/**
	 * �����ø����� MIME �ʼ��е��ļ�ͷ��Ϣ
	 * @param transfer_encoding {const char*} ���뷽ʽ
	 * @param out {string&} �洢��������� append ��ʽ
	 */
	void build_header(const char* transfer_encoding, string& out);

private:
	string filepath_;
	string filename_;
	string ctype_;
	string cid_;
	string charset_;

	bool rfc2047_encode(const char* name, const char* charset, string& out);
};

} // namespace acl
