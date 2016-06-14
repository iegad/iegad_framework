#pragma once
#include "acl_cpp/acl_cpp_define.hpp"

struct HTTP_HDR;
struct HTTP_HDR_RES;
struct HTTP_RES;
struct HTTP_HDR_REQ;
struct HTTP_REQ;

namespace acl {

class string;
class zlib_stream;
class socket_stream;
class ostream;
class istream;
class http_header;

/**
 * ������ô���1���� HTTP �ͻ������������������ʱ��2���� HTTP ����˽���
 * �� HTTP �ͻ�������ʱ����һ����Ӧ�� HTTP �ͻ���������
 * �ÿͻ������������֧�ֳ�����
 */
class ACL_CPP_API http_client
{
public:
	/**
	 * ȱʡ�Ĺ��캯����ʹ�ô˹��캯�������� HTTP �ͻ��˶�����Ҫ��ʾ��
	 * ���� http_client::open ����������
	 */
	http_client(void);

	/**
	 * �����Ѿ����ӳɹ������������󴴽� HTTP �ͻ��˶��󣬵���Ҫע����ǣ�
	 * ���� http_client ��������ʱ������� client �����󲢲��ᱻ���٣���
	 * ҪӦ���Լ����٣�����������Դй¶
	 * @param client {socket_stream*} HTTP ���������󣬿���������˵�����
	 *  Ҳ��������Ӧ�˵������ڱ����������ʱ�������󲢲��ᱻ���٣�����
	 *  �û��������ͷ�֮
	 * @param rw_timeout {int} IO ��д��ʱʱ��(��)
	 * @param is_request {bool} ������˻�����Ӧ�˵Ŀͻ�����
	 * @param unzip {bool} ��������ȡ����������Ӧ����ʱ��������������ص�
	 *  ������Ϊѹ������ʱ���ò��������ڵ�������ĺ���ʱ�Ƿ��Զ���ѹ��:
	 *  read_body(string&, bool, int*)
	 */
	http_client(socket_stream* client, int rw_timeout = 120,
		bool is_request = false, bool unzip = true);

	virtual ~http_client(void);

	/**
	 * ��֧�ֳ����ӵĶ�������У������ֹ����ô˺�������м�����ݶ���
	 * ��Ȼ�ⲻ�Ǳ���ģ���Ϊ�ڶ�ε��� read_head ʱ��read_head ���Զ�
	 * ���� reset ������ϴ���������е��Ǽ����
	 */
	void reset(void);

	/**
	 * ����Զ�� HTTP ������
	 * @param addr {const char*} ��������ַ����ʽ��IP:PORT �� DOMAIN:PORT
	 * @param conn_timeout {int} ���ӳ�ʱʱ��(��)
	 * @param rw_timeout {int} ��д��ʱʱ��(��)
	 * @param unzip {bool} �����������ص�������Ϊѹ������ʱ�Ƿ��Զ���ѹ��
	 * @return {bool} �����Ƿ�ɹ�
	 */
	bool open(const char* addr, int conn_timeout = 60, int rw_timeout = 60,
		bool unzip = true);

	/**
	 * д HTTP ����ͷ�������������
	 * @param header {http_header&}
	 * @return {bool} дͷ�������Ƿ�ɹ�
	 */
	bool write_head(const http_header& header);

	/**
	 * ���� HTTP �����壬����ѭ�����ô˺��������ڵ�һ�ε��� write ����д��
	 * HTTP ͷʱ������ chunked ���䷽ʽ�����ڲ��Զ����� chunked ���䷽ʽ; 
	 * ���⣬��ʹ�� chunked ��ʽ��������ʱ��Ӧ������ٵ���һ�α��������Ҳ�
	 * ������Ϊ 0 ��ʾ���ݽ���
	 * @param data {const void*} ���ݵ�ַ
	 * @param len {size_t} data ���ݳ���
	 * @return {bool} �����Ƿ�ɹ���������� false ��ʾ�����ж�
	 */
	bool write_body(const void* data, size_t len);

	/**
	 * ������ http_client(socket_stream*, bool) ���캯������
	 * ���� http_client(void) ����ͬʱ���� open ��������ʱ
	 * ���Ե��ñ��������������������
	 * @return {ostream&} ��������������ã���������������ڣ�
	 *  ���ڲ��Զ���������ԣ���ʾʹ����Ӧ�Ƚ�����
	 */
	ostream& get_ostream(void) const;

	/**
	 * ������ http_client(socket_stream*, bool) ���캯������
	 * ���� http_client(void) ����ͬʱ���� open ��������ʱ
	 * ���Ե��ñ���������������������
	 * @return {istream&} ���������������ã���������������ڣ�
	 *  ���ڲ��Զ���������ԣ���ʾʹ����Ӧ�Ƚ�����
	 */
	istream& get_istream(void) const;

	/**
	 * ������ http_client(socket_stream*, bool) ���캯������
	 * ���� http_client(void) ����ͬʱ���� open ��������ʱ
	 * ���Ե��ñ�����������������
	 * @return {socket_stream&} �����������ã���������������ڣ�
	 *  ���ڲ��Զ���������ԣ���ʾʹ����Ӧ�Ƚ�����
	 */
	socket_stream& get_stream(void) const;

	/**
	 * �� HTTP ��������ȡ��Ӧͷ���ݻ�� HTTP �ͻ��˶�ȡ�������ݣ�
	 * �ڳ����ӵĶ�������У�������������Զ�����ϴε��м����ݶ���
	 * @return {bool} �Ƿ�ɹ�
	 */
	bool read_head(void);

	/**
	 * ��� HTTP ��������������Ӧ�������峤��
	 * @return {int64) ����ֵ��Ϊ -1 ����� HTTP ͷ�����ڻ�û�г����ֶ�
	 */
#if defined(_WIN32) || defined(_WIN64)
	__int64 body_length(void) const;
#else
	long long int body_length(void) const;
#endif

	/**
	 * ���ö���Ϊ�����������ʱ���ú������������ͷ�еĳ�����ʼ��ַ��������ַ
	 * @param range_from {long long int&} ƫ����ʼλ��
	 * @param range_to {long long int&} ƫ�ƽ���λ��
	 * @return {bool} �������Ƿֶ����������򷵻� false��
	 *  ���Ƿֶ������򷵻� true��ͬʱ�� range_from �� range_to ��ֵ
	 *  ע��range_from/range_to �±�� 0 ��ʼ
	 *  ���ݸ�ʽ��
	 *  Range: bytes={range_from}-{range_to} ��
	 *  Range: bytes={range_from}-
	 */
#if defined(_WIN32) || defined(_WIN64)
	bool request_range(__int64& range_from, __int64& range_to);
#else
	bool request_range(long long int& range_from, long long int& range_to);
#endif

	/**
	 * ���ö���Ϊ��Ӧ��������ʱ���ú����������Ӧͷ�еĳ�����ʼ��ַ��������ַ
	 * @param range_from {long long int&} ƫ����ʼλ��
	 * @param range_to {long long int&} ƫ�ƽ���λ��
	 * @param total {long long int} ����ܳ���
	 * @return {bool} �������Ƿֶ���Ӧ�����򷵻� false��
	 *  ���Ƿֶ���Ӧ�򷵻� true��ͬʱ�� range_from �� range_to ��ֵ
	 *  ע��range_from/range_to �±�� 0 ��ʼ
	 *  ���ݸ�ʽ��
	 *  Content-Range: bytes {range_from}-{range_to}/{total_length}
	 *  �磺Content-Range: bytes 2250000-11665200/11665201
	 */ 
#if defined(_WIN32) || defined(_WIN64)
	bool response_range(__int64& range_from, __int64& range_to,
		__int64& total);
#else
	bool response_range(long long int& range_from,
		long long int& range_to, long long int& total);
#endif

	/**
	 * HTTP ������(����������Ӧ���Ƿ������ֳ�����)
	 * @return {bool}
	 */
	bool keep_alive(void) const;

	/**
	 * ��� HTTP ����ͷ����Ӧͷ��ĳ���ֶ������ֶ�ֵ
	 * @param name {const char*} �ֶ���
	 * @return {const char*} �ֶ�ֵ��Ϊ��ʱ��ʾ������
	 */
	const char* header_value(const char* name) const;

	/**
	 * ��ֹ HTTP ����/��Ӧͷ�е�ĳЩ�ֶ�
	 * @param name {const char*} �ֶ���
	 */
	void header_disable(const char* name);

	/**
	 * �� HTTP ͷ�е�ĳ���ֶν����滻
	 * @param name {const char*} HTTP ͷ���ֶ������磺Content-Length��
	 *   ���ֶβ����ִ�Сд
	 * @param value {const char*} ��ͷ���ֶε�ֵ
	 * @param force {bool} �����ͷ���ֶβ������Ƿ���Ҫǿ�����
	 * @return {bool} ���� false ��ʾ���������ͷ���ֶ����������Ҳ���
	 *  force_add Ϊ false
	 */
	bool header_update(const char* name, const char* value,
		bool force_add = true);

	/**
	 * �� HTTP ͷ�е�ĳ���ֶ��а���ĳ���ַ�����Դ�ַ��������滻, ����
	 * ֧�ֶ��ƥ���滻
	 * @param name {const char*} HTTP ͷ���ֶ������磺Content-Length��
	 *   ���ֶβ����ִ�Сд
	 * @param match {const char*} �ֶ�ֵ��ƥ����ַ���
	 * @param to {const char*} �滻�ɵ�Ŀ���ַ���ֵ
	 * @param case_sensitive {bool} �ڲ����滻ʱ�Ƿ����ִ�Сд
	 * @return {int} ƥ���滻�Ĵ�����0 ��ʾδ���κ��滻��< 0 ��ʾ����
	 */
	int header_update(const char* name, const char* match,
		const char* to, bool case_sensitive = false);

	/**
	 * ��� HTTP ���������ص� HTTP ��Ӧ״̬��
	 * 1xx, 2xx, 3xx, 4xx, 5xx
	 * @return {int} ������ֵΪ -1 ���ʾ������ûỰ����
	 *  ������ HTTP �������������ݹ���
	 */
	int response_status(void) const;

	/**
	 * ��� HTTP �ͻ�������� HOST �ֶ�ֵ
	 * @return {const char*} ���� NULL ��ʾ�����ڸ��ֶ�
	 */
	const char* request_host(void) const;

	/**
	 * ��� HTTP �ͻ�������� PORT �˿ں�
	 * @return {int} ���� -1 ��ʾ������
	 */
	int request_port(void) const;

	/**
	 * ��� HTTP �ͻ�������� HTTP ������GET, POST, CONNECT
	 * @return {const char*} ����ֵΪ�ձ�ʾ������
	 */
	const char* request_method(void) const;

	/**
	 * ��� HTTP �ͻ�������� URL �г�ȥ HTTP://domain �������
	 * �磺���� http://test.com.cn/cgi-bin/test?name=value�����
	 * ����Ӧ�÷��أ�/cgi-bin/test?name=value
	 * @return {const char*} ���� NULL ��ʾ������
	 */
	const char* request_url(void) const;

	/**
	 * ��� HTTP �ͻ�������� URL �е����·��(��������������)��
	 * �磺���� http://test.com.cn/cgi-bin/test?name=value�����
	 * ����Ӧ�÷��أ�/path/test.cgi
	 * @return {const char*} ���� NULL ��ʾ������
	 */
	const char* request_path(void) const;

	/**
	 * ��� HTTP �ͻ�������� URL �е����в������磺
	 * http://test.com.cn/cgi-bin/test?name=value����ú���Ӧ�÷��أ�
	 * name=value
	 * @return {const char*} ���� NULL ��ʾ������
	 */
	const char* request_params(void) const;

	/**
	 * ��� HTTP �ͻ�������� URL ��ָ���Ĳ���ֵ���磺
	 * http://test.com.cn/cgi-bin/test?name=value����ͨ���ú�������
	 * ��� name ������ֵΪ value
	 * @param name {const char*} ������
	 * @return {const char*} ����ֵ������ NULL ��ʾ������
	 */
	const char* request_param(const char* name) const;

	/**
	 * ��� HTTP �ͻ�������ͷ�е� cookie ֵ
	 * @param name {const char*} cookie ��
	 * @return {const char*} cookie ֵ������ NULL ���ʾ������
	 */
	const char* request_cookie(const char* name) const;

	/**
	 * �� HTTP ��������ȡ��Ӧ�����ݻ�� HTTP �ͻ��˶�ȡ���������ݣ�
	 * �˺��������յ����������ݽ��н�ѹ����
	 * @param out {string&} �洢������Ļ�����
	 * @param clean {bool} �ڽ�������ǰ�Ƿ��Զ���� buf ������
	 * @param real_size {int*} ����ָ��ǿգ����¼�������������ݳ��ȣ�
	 *  ͨ����ָ�뷵�ص�����ֵ��Զ >= 0
	 * @return {int} ����ֵ�������£�(Ӧ����Ҫͨ�� body_finish ������
	 *       disconnected �������ж��������Ƿ����������Ƿ�ر�)
	 *  > 0: ��ʾ�Ѿ����������ݣ��������ݻ�δ����
	 *  == 0: ������ԭ��᷵�� 0�������ݶ���ʱ���� 0���ɵ��� body_finish
	 *        �����ж��Ƿ��Ѿ����� HTTP ��Ӧ���ݣ�������ѹ�����ݵ�β��ʱ��
	 *        ��ѹ�����ݵ�8�ֽ�β�������ǿ����ֶΣ����Բ���Ϊ�����巵�أ�
	 *        ��ʱҲ�᷵�� 0��
	 *        ������ͨ�� disconnected() �����ж������Ƿ��Ѿ����ر�
	 *        ������ݶ��������Ӱ�δ�رգ�����Լ������ֳ�����
	 *  < 0: ��ʾ���ӹر�
	 * ע��read_body �������������ܻ��ã�
	 *     ��Ϊ��ѹ������ʱ���򷵻ص�ֵΪ��ѹ��������ݳ���
	 */
	int read_body(string& out, bool clean = true, int* real_size = NULL);
	
	/**
	 * �� HTTP ��������ȡ��Ӧ�����ݻ�� HTTP �ͻ��˶�ȡ���������ݣ�
	 * �ú������ܶ����ݽ��н�ѹ
	 * @param buf {char*} �洢������Ļ�����������Ϊ��
	 * @param size {size_t} buf ����������
	 * @return {int} ����ֵ�������£�
	 *  > 0: ��ʾ�Ѿ����������ݣ��������ݻ�δ����
	 *  == 0: ��ʾ�Ѿ����� HTTP ��Ӧ�����ݣ������Ӳ�δ�ر�
	 *  < 0: ��ʾ���ӹر�
	 */
	int read_body(char* buf, size_t size);

	/**
	 * �� HTTP ��������Ӧ���ݻ�ͻ������������ж�ȡһ�����ݣ��˺����ڲ���
	 * ���ԭʼ���ݽ��н�ѹ����������ѭ�����ô˺���ֱ���ú������� false
	 * �� body_finish() ���� true Ϊֹ�����ú������� false ʱ��ʾ�����Ѿ�
	 * �رգ������� true ʱ��ʾ������һ�����ݣ���ʱ����ͨ���ж�
	 * body_finish() ����ֵ���ж��Ƿ��Ѿ�������������
	 * @param out {string&} �洢������Ļ��������ڸú����ڲ������Զ������
	 *  ���������û����ڵ��øú���ǰ��������û�����(�ɵ���:out.clear())
	 * @param nonl {bool} ��ȡһ������ʱ�Ƿ��Զ�ȥ��β���� "\r\n" �� "\n"
	 * @param size {size_t*} ������������һ������ʱ��Ÿ������ݵĳ��ȣ�
	 *  ������һ�������� nonl Ϊ true ʱ�����ֵΪ 0
	 * @return {bool} �Ƿ������һ�����ݣ����ú������� false ʱ��ʾ�����
	 *  ���������û�ж���������һ�����ݣ�������� true ��ʾ������һ��
	 *  ���ݣ�������һ������ʱ�ú���Ҳ�᷵�� true��ֻ�� *size = 0
	 */
	bool body_gets(string& out, bool nonl = true, size_t* size = NULL);

	/**
	 * �ж��Ƿ��Ѿ����� HTTP ��Ӧ������
	 * @return {bool}
	 */
	bool body_finish(void) const;

	/**
	 * �ж����������Ƿ��Ѿ��ر�
	 * @return {bool}
	 */
	bool disconnected(void) const;

	/**
	 * ȡ��ͨ�� read_head ������ HTTP ��Ӧͷ�����ҵ����뻺����
	 * �ǿ�ʱ���� HTTP ��Ӧͷ���ݿ�����������
	 * @param buf {string*} �ǿ�ʱ�����洢 HTTP ��Ӧͷ����
	 * @return {const HTTP_HDR_RES*} HTTP ��Ӧͷ�������Ϊ�գ���˵��
	 *  δ������Ӧͷ����
	 */
	HTTP_HDR_RES* get_respond_head(string* buf);

	/**
	 * ȡ��ͨ�� read_head ������ HTTP ����ͷ�����ҵ����뻺����
	 * �ǿ�ʱ���� HTTP ����ͷ���ݿ�����������
	 * @param buf {string*} �ǿ�ʱ�����洢 HTTP ����ͷ����
	 * @return {const HTTP_HDR_REQ*} HTTP ����ͷ�������Ϊ�գ���˵��
	 *  δ��������ͷ����
	 */
	HTTP_HDR_REQ* get_request_head(string* buf);

	/**
	 * ������������ص� HTTP ��Ӧͷ��Ϣ����׼���
	 * @param prompt {const char*} ���ǿ�����ͬ HTTP ͷ��Ϣһ�����
	 */
	void print_header(const char* prompt = NULL);

	/**
	 * ������������ص� HTTP ��Ӧͷ��Ϣ���������
	 * @param out {ostream&} ��������������ļ�����Ҳ������������
	 * @param prompt {const char*} ���ǿ�����ͬ HTTP ͷ��Ϣһ�����
	 */
	void fprint_header(ostream& out, const char* prompt = NULL);

	/**
	 * ������������ص� HTTP ��Ӧͷ��Ϣ����������
	 * @param out {string&} �洢��������ݻ�����
	 * @param prompt {const char*} ���ǿ�����ͬ HTTP ͷ��Ϣһ�����
	 */
	void sprint_header(string& out, const char* prompt = NULL);

private:
	socket_stream* stream_;     // HTTP ������
	bool stream_fixed_;         // �Ƿ������ͷ� stream_ ������

	HTTP_HDR_RES* hdr_res_;     // HTTP ͷ��Ӧ����
	struct HTTP_RES* res_;      // HTTP ��Ӧ����
	HTTP_HDR_REQ* hdr_req_;     // HTTP ͷ�������
	struct HTTP_REQ* req_;      // HTTP �������
	int  rw_timeout_;           // IO ��д��ʱʱ��
	bool unzip_;                // �Ƿ��ѹ�����ݽ��н�ѹ��
	zlib_stream* zstream_;      // ��ѹ����
	bool is_request_;           // �Ƿ��ǿͻ������
	int  gzip_header_left_;     // gzip ͷʣ��ĳ���
	int  last_ret_;             // ���ݶ�����¼���ķ���ֵ
	bool head_sent_;            // ͷ�������Ƿ��Ѿ��������
	bool body_finish_;          // �Ƿ��Ѿ����� HTTP ��Ӧ������
	bool disconnected_;         // ���������Ƿ��Ѿ��ر�
	bool chunked_transfer_;     // �Ƿ�Ϊ chunked ����ģʽ
	unsigned gzip_crc32_;       // gzip ѹ������ʱ�ļ���ֵ
	unsigned gzip_total_in_;    // gzip ѹ��ǰ�������ݳ���      
	string* buf_;               // �ڲ������������ڰ��ж��Ȳ�����

	bool read_request_head(void);
	bool read_response_head(void);
	int  read_request_body(char* buf, size_t size);
	int  read_response_body(char* buf, size_t size);
	int  read_request_body(string& out, bool clean, int* real_size);
	int  read_response_body(string& out, bool clean, int* real_size);

	HTTP_HDR* get_http_hdr() const;

public:
	bool write_chunk(ostream& out, const void* data, size_t len);
	bool write_chunk_trailer(ostream& out);

	bool write_gzip(ostream& out, const void* data, size_t len);
	bool write_gzip_trailer(ostream& out);
};

}  // namespace acl
