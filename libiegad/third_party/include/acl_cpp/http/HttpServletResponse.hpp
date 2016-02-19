#pragma once
#include "acl_cpp/acl_cpp_define.hpp"

namespace acl {

class dbuf_guard;
class string;
class ostream;
class socket_stream;
class http_header;
class http_client;
class HttpCookie;
class HttpServletRequest;

/**
 * �� HTTP �ͻ�����Ӧ��ص��࣬���಻Ӧ���̳У��û�Ҳ����Ҫ
 * ����򴴽��������
 */
class ACL_CPP_API HttpServletResponse
{
public:
	/**
	 * ���캯��
	 * @param stream {socket_stream&} ���������ڲ������Զ��ر���
	 */
	HttpServletResponse(socket_stream& stream);
	~HttpServletResponse(void);

	/**
	 * ���� HTTP ��Ӧ������ĳ���
	 * @param n {acl_int64} �����峤��
	 */
#if defined(_WIN32) || defined(_WIN64)
	HttpServletResponse& setContentLength(__int64 n);
#else
	HttpServletResponse& setContentLength(long long int n);
#endif

	/**
	 * ���� HTTP chunked ����ģʽ
	 * @param on {bool} ���Ϊ true����ʹ������ setContentLength��
	 *  ���ڲ�Ҳ����� chunked ���䷽ʽ������ HTTP RFC �淶Ҫ��
	 *  chunked ��������ȼ��߼� conteng-length ��ʽ
	 * @return {HttpServletResponse&}
	 */
	HttpServletResponse& setChunkedTransferEncoding(bool on);

	/**
	 * ������ HTTP �ͻ��˱�����ϵ������
	 * @param on {bool}
	 * @return {HttpServletResponse&}
	 */
	HttpServletResponse& setKeepAlive(bool on);

	/**
	 * ���� HTTP ��Ӧ������� Content-Type �ֶ�ֵ�����ֶ�ֵ����Ϊ��
	 * text/html �� text/html; charset=utf8 ��ʽ
	 * @param value {const char*} �ֶ�ֵ
	 * @return {HttpServletResponse&}
	 */
	HttpServletResponse& setContentType(const char* value);

	/**
	 * ���� HTTP ��Ӧ��������� gzip ѹ����ʽ
	 * @param gzip {bool} �Ƿ���� gzip ѹ����ʽ
	 * @return {HttpServletResponse&}
	 */
	HttpServletResponse& setContentEncoding(bool gzip);

	/**
	 * ���� HTTP ��Ӧ���������ַ��������Ѿ��� setContentType ����
	 * ���ַ�������Ͳ����ٵ��ñ����������ַ���
	 * @param charset {const char*} ��Ӧ�����ݵ��ַ���
	 * @return {HttpServletResponse&}
	 */
	HttpServletResponse& setCharacterEncoding(const char* charset);

	/**
	 * ���� HTTP ��Ӧͷ�е����ڸ�ʽ���ֶ�
	 * @param name {const char*} HTTP ��Ӧͷ�е��ֶ���
	 * @param value {time_t} ʱ��ֵ
	 */
	HttpServletResponse& setDateHeader(const char* name, time_t value);

	/**
	 * ���� HTTP ��Ӧͷ�е��ַ�����ʽ�ֶ�
	 * @param name {const char*} HTTP ��Ӧͷ�е��ֶ���
	 * @param value {const char*} �ֶ�ֵ
	 */
	HttpServletResponse& setHeader(const char* name, const char* value);

	/**
	 * ���� HTTP ��Ӧͷ�е�������ʽ�ֶ�
	 * @param name {const char*} HTTP ��Ӧͷ�е��ֶ���
	 * @param value {int} �ֶ�ֵ
	 */
	HttpServletResponse& setHeader(const char* name, int value);

	/**
	 * ���ڷ������أ����ñ����������������ص�ƫ��λ�ã��±�� 0 ��ʼ��
	 * @param from {http_off_t} ����������ʼƫ��λ�ã��±�� 0 ��ʼ���㣩
	 * @param to {http_off_t} �����������λ�ã���ֵ��С�������ݳ��ȣ�
	 * @param total {http_off_t} �����ݳ��ȣ�������ԴΪһ����̬�ļ�ʱ��ֵ
	 *  Ӧ���ڸ��ļ����ܳ��ȴ�С
	 * @return {HttpServletResponse&}
	 */
#if  defined(_WIN32) || defined(_WIN64)
	HttpServletResponse& setRange(__int64 from,
		__int64 to, __int64 total);
#else
	HttpServletResponse& setRange(long long from,
		long long to, long long total);
#endif

	/**
	 * ���� HTTP ��Ӧͷ�е�״̬�룺1xx, 2xx, 3xx, 4xx, 5xx
	 * @param status {int} HTTP ��Ӧ״̬��, �磺200
	 */
	HttpServletResponse& setStatus(int status);

	/**
	 * ����Ϊ CGI ģʽ���û�һ�㲻���ֹ����ã���Ϊ HttpServlet ��
	 * ���Զ������Ƿ��� CGI ģʽ
	 * @param on {bool} �Ƿ��� CGI ģʽ
	 */
	HttpServletResponse& setCgiMode(bool on);

	/**
	 * ���� HTTP ��Ӧͷ�е��ض��� location �ֶ�
	 * @param location {const char*} URL���ǿ�
	 * @param status {int} HTTP ��Ӧ״̬�룬һ��Ϊ 3xx ��
	 */
	HttpServletResponse& setRedirect(const char* location, int status = 302);

	/**
	 * ��� cookie ���󣬸ö�������Ƕ�̬����ģ����û��Լ�����
	 * ����ʾ�ͷŸö�����Ϊ�ڲ����Զ��ͷ�
	 * @param cookie {HttpCookie*}
	 */
	HttpServletResponse& addCookie(HttpCookie* cookie);

	/**
	 * ��� cookie
	 * @param name {const char*} cookie ��
	 * @param value {const char*} cookie ֵ
	 * @param domain {const char*} cookie �洢��
	 * @param path {const char*} cookie �洢·��
	 * @param expires {time_t} cookie ����ʱ����������ǰʱ���
	 *  ��ֵΪ cookie �Ĺ���ʱ���(��)
	 */
	HttpServletResponse& addCookie(const char* name, const char* value,
		const char* domain = NULL, const char* path = NULL,
		time_t expires = 0);

	/**
	 * �� url ���� url ����
	 * @param out {string&} �洢�����Ľ��
	 * @param url {const char*} δ����ǰԭʼ�� url
	 */
	void encodeUrl(string& out, const char* url);

	/**
	 * ��� HTTP ��Ӧͷ
	 * @return {http_header&}
	 */
	http_header& getHttpHeader(void) const;

	/**
	 * ��ͻ��˷��� HTTP ��������Ӧ���ݣ�����ѭ�����ô˺�����
	 * ��ͨ�� setChunkedTransferEncoding ������ chunked ���䷽ʽ��
	 * �ڲ��Զ����� chunked ���䷽ʽ�����ô˺���������ʽ����
	 * sendHeader ���������� HTTP ��Ӧͷ����Ϊ�ڲ����Զ��ڵ�һ��
	 * дʱ���� HTTP ��Ӧͷ�����⣬��ʹ�� chunked ��ʽ��������ʱ��
	 * Ӧ��Ӧ������ٵ���һ�α��������Ҳ�������Ϊ 0 ��ʾ���ݽ���
	 * @param data {const void*} ���ݵ�ַ
	 * @param len {size_t} data ���ݳ���
	 * @return {bool} �����Ƿ�ɹ���������� false ��ʾ�����ж�
	 */
	bool write(const void* data, size_t len);

	/**
	 * ��ͻ��˷��� HTTP ��������Ӧ���ݣ�����ѭ�����ô˺������ú���
	 * �ڲ����� HttpServletResponse::write(const void*, size_t) ���̣�
	 * ���⣬��ʹ�� chunked ��ʽ��������ʱ��Ӧ��Ӧ������ٵ���һ�α�������
	 * ������մ����� buf.empty() == true
	 * @param buf {const string&} ���ݻ�����
	 * @return {bool} �����Ƿ�ɹ���������� false ��ʾ�����ж�
	 */
	bool write(const string& buf);

	/**
	 * ����ʽ��ʽ�� HTTP �ͻ��˷�����Ӧ���ݣ��ڲ��Զ�����
	 * HttpServletResponse::write(const void*, size_t) ���̣���ʹ��
	 * chunked ��ʽ��������ʱ��Ӧ��Ӧ������ٵ��� write(NULL, 0)
	 * ��ʾ���ݽ���
	 * @param fmt {const char*} ��θ�ʽ�ַ���
	 * @return {int} �ɹ��򷵻�ֵ > 0�����򷵻� -1
	 */
	int format(const char* fmt, ...) ACL_CPP_PRINTF(2, 3);

	/**
	 * ����ʽ��ʽ�� HTTP �ͻ��˷�����Ӧ���ݣ��ڲ��Զ�����
	 * HttpServletResponse::write(const string&) ���̣���ʹ�� chunked
	 * ��ʽ��������ʱ��Ӧ��Ӧ������ٵ��� write(NULL, 0) ��ʾ���ݽ���
	 * @param fmt {const char*} ��θ�ʽ�ַ���
	 * @param ap {va_list} ����б�
	 * @return {int} �ɹ��򷵻�ֵ > 0�����򷵻� -1
	 */
	int vformat(const char* fmt, va_list ap);

	///////////////////////////////////////////////////////////////////

	/**
	 * ���� HTTP ��Ӧͷ���û�Ӧ�÷���������ǰ���ô˺����� HTTP
	 * ��Ӧͷ���͸��ͻ���
	 * @return {bool} �����Ƿ�ɹ��������� false ���ʾ�����жϣ�
	 *  ���������ϼ���д�ĺ���ʱ��������������ʽ�����ã������
	 *  ͨ���� getOutputStream ��õ� socket ��д����ʱ���򱾺���
	 *  ������ʽ������
	 */
	bool sendHeader(void);

	/**
	 * ��� HTTP ��Ӧ���������������û��ڵ��� sendHeader ����
	 * �� HTTP ��Ӧͷ��ͨ��������������� HTTP ������
	 * @return {ostream&}
	 */
	ostream& getOutputStream(void) const;

	/**
	 * ���� http ������󣬸ú���ĿǰֻӦ�� HttpServlet ���ڲ�����
	 * @param request {HttpServletRequest*}
	 */
	void setHttpServletRequest(HttpServletRequest* request);

	/**
	 * ��õײ�� http_client ͨ�Ŷ���
	 * @return {http_client*} �� NULL
	 */
	http_client* getClient() const
	{
		return client_;
	}

private:
	dbuf_guard* dbuf_internal_;
	dbuf_guard* dbuf_;
	socket_stream& stream_;		// �ͻ���������
	HttpServletRequest* request_;	// http �������
	http_client* client_;		// http ��Ӧ������
	http_header* header_;		// http ��Ӧͷ
	char  charset_[32];		// �ַ���
	char  content_type_[32];	// content-type ����
	bool  head_sent_;		// �Ƿ��Ѿ������� HTTP ��Ӧͷ
};

}  // namespace acl
