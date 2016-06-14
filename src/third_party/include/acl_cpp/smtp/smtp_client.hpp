#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/stream/socket_stream.hpp"
#include <vector>

struct SMTP_CLIENT;

namespace acl {

class istream;
class polarssl_conf;
class mail_message;

/**
 * SMTP �ʼ����Ϳͻ����࣬����ʹ�ô���������ʼ���֧�������֤�ȹ���
 */
class ACL_CPP_API smtp_client
{
public:
	/**
	 * ���캯��
	 * @param addr {const char*} SMTP �ʼ���������ַ����ʽ��IP:PORT
	 *  �� domain:port
	 * @param conn_timeout {int} ���ӷ������ĳ�ʱʱ��(�룩
	 * @param rw_timeout {int} ���� IO ��ʱʱ��(��)
	 */
	smtp_client(const char* addr, int conn_timeout = 60,
		int rw_timeout = 60);
	~smtp_client();

	/**
	 * ���ñ����������ʼ��������ʼ�����ˣ��ú��������ȵ��� send_envelop
	 * �����ŷ⣬�� email �� message.get_email() �ǿ�ʱ�������÷����ʼ�
	 * ���̣����򣨼� email �� message.get_email() ��Ϊ NULL����ֻ����
	 * �ŷ�
	 * @param message {const mail_messsage&} �ʼ������Ϣ��������ǰ������
	 * @param email {const char*} �ǿ�ʱ������ʹ�ô��ļ���Ϊ�ʼ������ݷ���
	 * @return {bool} �����Ƿ�ɹ�
	 *  ע����� email Ϊ NULL ͬʱ messsage.get_email() ҲΪ NULL����
	 *     ���������� SMTP �ŷⲿ�֣��û�����Ҫ���ã�
	 *     data_begin-->write|format|vformat|send_file-->data_end
	 *     �����������ʼ�������
	 */
	bool send(const mail_message& message, const char* email = NULL);

	/**
	 * �� SMTP �Ự�׶ν������ʼ��ŷⲿ�����ݣ�Ӧ�õ��ô˺����ɹ���
	 * ����Ҫ����:
	 * 1��data_begin����ʼ�����ʼ���ָ��
	 * 2��write/format/vformat/send_file�������ʼ�����
	 * 3��data_end����ʾ�����ʼ����������
	 * @param message {const mail_message&} �����ʼ����������ʼ���Ϣ����
	 * @return {bool} �Ƿ�ɹ�
	 *  ע���������� open/auth_login/mail_from/rcpt_to �����ŷ���̵����
	 */
	bool send_envelope(const mail_message& message);

	/**
	 * ���� SSL ���ݴ���ģʽ
	 * @param ssl_conf {polarssl_conf*} �ǿ�ʱ��ָ������ SSL ����ģʽ
	 * @return {smtp_client&}
	 */
	smtp_client& set_ssl(polarssl_conf* ssl_conf);

	/**
	 * ����ϴ� SMTP �������̷���˷��ص�״̬��
	 * @return {int}
	 */
	int get_code() const;

	/**
	 * ����ϴ� SMTP �������̷���˷��ص�״̬��Ϣ
	 * @return {const char*}
	 */
	const char* get_status() const;

	/////////////////////////////////////////////////////////////////////

	/**
	 * �����ʼ������ݣ�����ѭ�����ñ����������������ݱ������ϸ���ʼ���ʽ
	 * @param data {const char*} �ʼ�����
	 * @param len {size_t} data �ʼ����ݳ���
	 * @return {bool} ��������Ƿ�ɹ�
	 *  ע���ڵ�һ�ε��ñ�����ǰ�����뱣֤ SMTP �ŷ��Ѿ��ɹ�����
	 */
	bool write(const char* data, size_t len);

	/**
	 * �����ʼ������ݣ�����ѭ�����ñ����������������ݱ������ϸ���ʼ���ʽ
	 * @param fmt {const char*} ��θ�ʽ
	 * @return {bool} ��������Ƿ�ɹ�
	 *  ע���ڵ�һ�ε��ñ�����ǰ�����뱣֤ SMTP �ŷ��Ѿ��ɹ�����
	 */
	bool format(const char* fmt, ...);

	/**
	 * �����ʼ������ݣ�����ѭ�����ñ����������������ݱ������ϸ���ʼ���ʽ
	 * @param fmt {const char*} ��θ�ʽ
	 * @param ap {va_list}
	 * @return {bool} ��������Ƿ�ɹ�
	 *  ע���ڵ�һ�ε��ñ�����ǰ�����뱣֤ SMTP �ŷ��Ѿ��ɹ�����
	 */
	bool vformat(const char* fmt, va_list ap);

	/////////////////////////////////////////////////////////////////////

	/**
	 * ����Զ�� SMTP ������
	 * @return {bool} �����Ƿ�ɹ�������ʹ�� SSL ��ʽ������Ҫ�������
	 *  ��ʼ������� set_ssl ���� SSL ͨ�ŷ�ʽ
	 */
	bool open();

	/**
	 * �����ر��� SMTP ����˵�����
	 */
	void close();

	/**
	 * ��һ�����ӳɹ�����Ҫ���ñ�������� SMTP ����˵Ļ�ӭ��Ϣ
	 * @return {bool} �Ƿ�ɹ�
	 */
	bool get_banner();

	/**
	 * ���� get_banner �ɹ�����ñ������� SMTP ����˷��� HELO/HELO ����
	 * @return {bool} �Ƿ�ɹ�
	 */
	bool greet();

	/**
	 * ���� gree �ɹ�����ñ������� SMTP ����˷��������֤����
	 * @param user {const char*} �û��˺ţ��ǿ��ַ���
	 * @param pass {const char*} �û��˺����룬�ǿ��ַ���
	 * @return {bool} �Ƿ�ɹ�
	 */
	bool auth_login(const char* user, const char* pass);

	/**
	 * ���� auth_login �ɹ�������������֤��������� greet �ɹ���
	 * ���ñ������� SMTP ���������� MAIL FROM ����
	 * @param from {const char*} �����˵������ַ
	 * @return {bool} �Ƿ�ɹ�
	 */
	bool mail_from(const char* from);

	/**
	 * ���� mail_from �ɹ�����ñ������� SMTP ����˷��� RCPT TO ���
	 * ָ��һ���ռ��ˣ����Զ�α��������ʼ�����������ռ���
	 * @param to {const char*} �ռ��������ַ
	 * @return {bool} �Ƿ�ɹ�
	 */
	bool rcpt_to(const char* to);

	/**
	 * ���� rcpt_to �� send_envelope �ɹ����ñ������� SMTP �����
	 * DATA ���������ʼ�����ʼ�����
	 * @return {bool} ��������Ƿ�ɹ�
	 *  ע���ڵ��ñ�����ǰ�����뱣֤ SMTP �ŷ��Ѿ��ɹ�����
	 */
	bool data_begin();

	/**
	 * ���� data_begin �ɹ����ñ������� SMTP ����˷���һ���������ʼ���
	 * ��Ҫ�����ʼ��洢�ڴ����ϵ�·��
	 * @param filepath {const char*} �ʼ��ļ�·��
	 * @return {bool} ��������Ƿ�ɹ�
	 *  ע���ڵ��ñ�����ǰ�����뱣֤ SMTP �ŷ��Ѿ��ɹ�����
	 */
	bool send_email(const char* filepath);

	/**
	 * �ʼ�������ϣ�����ã�send_email������������ñ��������� SMTP
	 * �ʼ��������������ݽ���
	 * @return {bool} ��������Ƿ�ɹ�
	 */
	bool data_end();

	/**
	 * �Ͽ����ʼ�������������
	 * @return {bool} ��������Ƿ�ɹ�
	 */
	bool quit();

	/**
	 * NOOP ����
	 * @return {bool} ��������Ƿ�ɹ�
	 */
	bool noop();

	/**
	 * �������ʼ�������������״̬
	 * @return {bool} ��������Ƿ�ɹ�
	 */
	bool reset();

	/**
	 * ����� SMTP ������֮������������󣬸ú���ֻ�е� open �ɹ���ſɵ���
	 * @return {socket_stream&}
	 */
	socket_stream& get_stream(void)
	{
		return stream_;
	}

private:
	polarssl_conf* ssl_conf_;
	char* addr_;
	int   conn_timeout_;
	int   rw_timeout_;
	SMTP_CLIENT* client_;
	socket_stream stream_;
	bool  ehlo_;
	bool  reuse_;

	bool to_recipients(const std::vector<rfc822_addr*>& recipients);
};

} // namespace acl
