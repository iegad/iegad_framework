#pragma once
#include "acl_cpp/acl_cpp_define.hpp"
#include "acl_cpp/stdlib/log.hpp"
#include "acl_cpp/http/http_header.hpp"

namespace acl {

class session;
class socket_stream;
class HttpServletRequest;
class HttpServletResponse;

/**
 * ���� HTTP �ͻ�������Ļ��࣬������Ҫ�̳и���
 */
class ACL_CPP_API HttpServlet
{
public:
	/**
	 * ���캯��
	 * @param stream {socket_stream*} ���� acl_master ��������ܿ�����
	 *  ����ʱ���ò�������ǿգ����� apache ���� CGI ��ʽ����ʱ���ò���
	 *  ��Ϊ NULL�����⣬�ú����ڲ�����ر������ӣ�Ӧ��Ӧ���д���������
	 *  �Ĺر�������������Է����� acl_master �ܹ����
	 * @param session {session*} ÿһ�� HttpServlet ����һ�� session ����
	 */
	HttpServlet(socket_stream* stream, session* session);

	/**
	 * ���캯��
	 * @param stream {socket_stream*} ���� acl_master ��������ܿ�����
	 *  ����ʱ���ò�������ǿգ����� apache ���� CGI ��ʽ����ʱ���ò���
	 *  ��Ϊ NULL�����⣬�ú����ڲ�����ر������ӣ�Ӧ��Ӧ���д���������
	 *  �Ĺر�������������Է����� acl_master �ܹ����
	 * @param memcache_addr {const char*}
	 */
	HttpServlet(socket_stream* stream,
		const char* memcache_addr = "127.0.0.1:11211");

	HttpServlet();
	virtual ~HttpServlet(void) = 0;

	session& getSession() const
	{
		return *session_;
	}

	socket_stream* getStream() const
	{
		return stream_;
	}

	/**
	 * ���ñ����ַ�������������˱����ַ��������ڽ��� HTTP ��������ʱ����
	 * �Զ���������ַ���תΪ�����ַ������ú��������� doRun ֮ǰ���ò���Ч
	 * @param charset {const char*} �����ַ����������ָ��Ϊ�գ�
	 *  ����������ַ���
	 * @return {HttpServlet&}
	 */
	HttpServlet& setLocalCharset(const char* charset);

	/**
	 * ���� HTTP �Ự���� IO ��д��ʱʱ�䣻�ú��������� doRun ǰ���ò���Ч
	 * @param rw_timeout {int} ��д��ʱʱ��(��)
	 * @return {HttpServlet&}
	 */
	HttpServlet& setRwTimeout(int rw_timeout);

	/**
	 * ��� POST �������÷��������Ƿ���Ҫ�������������ݣ�Ĭ��Ϊ�������ú�
	 * �������� doRun ֮ǰ���ò���Ч����������Ϊ�������� MIME ��ʽ����ʹ
	 * ���ñ����������˽������ݣ�Ҳ�������������н���
	 * @param on {bool} �Ƿ���Ҫ����
	 * @return {HttpServlet&}
	 */
	HttpServlet& setParseBody(bool on);

	/**
	 * ��� POST �������÷������ý������������󳤶ȣ���������壬�ú���
	 * ������ doRun ֮ǰ���ò���Ч
	 * @param length {int} ��󳤶����ƣ��������������峤�ȹ�����ֱ��
	 *  ���� false�������ֵ <= 0 ���ڲ������������峤�ȣ����øú���ǰ
	 *  �ڲ�ȱʡֵΪ 0
	 * @return {HttpServlet&}
	 */
	HttpServlet& setParseBodyLimit(int length);
	
	/**
	 * HttpServlet ����ʼ���У����� HTTP ���󣬲��ص����� doXXX �麯����
	 * @return {bool} ���ش����������� false ��ʾ����ʧ�ܣ���Ӧ�ر����ӣ�
	 *  ���� true ��ʾ����ɹ������ô˺�����Ӧ�ü���ͨ���ж�����/��Ӧ������
	 *  �Ƿ���Ҫ���ֳ�������ȷʵ�����Ƿ񱣳ֳ�����
	 */
	bool start(void);

	/**
	 * HttpServlet ����ʼ���У����� HTTP ���󣬲��ص����� doXXX �麯����
	 * �ú������Ȼ���� start ���̣�Ȼ����� start �ķ��ؽ��������/��Ӧ
	 * �����Ƿ�Ҫ�󱣳ֳ������������Ƿ���Ҫ��ͻ��˱��ֳ�����
	 * @return {bool} ���ش����������� false ��ʾ����ʧ�ܻ���ɹ��Ҳ�����
	 *  �����ӣ�Ӧ�ر�����
	 */
	bool doRun();

	/**
	 * HttpServlet ����ʼ���У����� HTTP ���󣬲��ص����� doXXX �麯��
	 * @param session {session&} �洢 session ���ݵĶ���
	 * @param stream {socket_stream*} ���� acl_master ��������ܿ�����
	 *  ����ʱ���ò�������ǿգ����� apache ���� CGI ��ʽ����ʱ���ò���
	 *  ��Ϊ NULL�����⣬�ú����ڲ�����ر������ӣ�Ӧ��Ӧ���д���������
	 *  �Ĺر�������������Է����� acl_master �ܹ����
	 * @return {bool} ���ش�����
	 */
	bool doRun(session& session, socket_stream* stream = NULL);

	/**
	 * HttpServlet ����ʼ���У����� HTTP ���󣬲��ص����� doXXX �麯����
	 * ���ñ�������ζ�Ų��� memcached ���洢 session ����
	 * @param memcached_addr {const char*} memcached ��������ַ����ʽ��IP:PORT
	 * @param stream {socket_stream*} ����ͬ��
	 * @return {bool} ���ش�����
	 */
	bool doRun(const char* memcached_addr, socket_stream* stream);

	/**
	 * �� HTTP ����Ϊ GET ��ʽʱ���麯��
	 */
	virtual bool doGet(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doGet yet!");
		return false;
	}

	/**
	 * �� HTTP ����Ϊ POST ��ʽʱ���麯��
	 */
	virtual bool doPost(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doPost yet!");
		return false;
	}

	/**
	 * �� HTTP ����Ϊ PUT ��ʽʱ���麯��
	 */
	virtual bool doPut(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doPut yet!");
		return false;
	}

	/**
	 * �� HTTP ����Ϊ CONNECT ��ʽʱ���麯��
	 */
	virtual bool doConnect(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doConnect yet!");
		return false;
	}

	/**
	 * �� HTTP ����Ϊ PURGE ��ʽʱ���麯�����÷�������� SQUID �Ļ���
	 * ʱ���õ�
	 */
	virtual bool doPurge(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doPurge yet!");
		return false;
	}

	/**
	 * �� HTTP ����Ϊ DELETE ��ʽʱ���麯��
	 */
	virtual bool doDelete(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doPurge yet!");
		return false;
	}

	/**
	 * �� HTTP ����Ϊ HEAD ��ʽʱ���麯��
	 */
	virtual bool doHead(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doPurge yet!");
		return false;
	}

	/**
	 * �� HTTP ����Ϊ OPTION ��ʽʱ���麯��
	 */
	virtual bool doOptions(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doPurge yet!");
		return false;
	}

	/**
	 * �� HTTP ����Ϊ PROPFIND ��ʽʱ���麯��
	 */
	virtual bool doPropfind(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doPurge yet!");
		return false;
	}

	/**
	 * �� HTTP ���󷽷�δ֪ʱ���麯��
	 * @param method {const char*} ����δ֪�����󷽷�
	 */
	virtual bool doOther(HttpServletRequest&, HttpServletResponse&,
		const char* method)
	{
		(void) method;
		logger_error("child not implement doOther yet!");
		return false;
	}

	/**
	 * �� HTTP ���󷽷�δ֪ʱ���麯��
	 */
	virtual bool doUnknown(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doUnknown yet!");
		return false;
	}

	/**
	 * �� HTTP �������ʱ���麯��
	 */
	virtual bool doError(HttpServletRequest&, HttpServletResponse&)
	{
		logger_error("child not implement doError yet!");
		return false;
	}

protected:
	HttpServletRequest* req_;
	HttpServletResponse* res_;

private:
	session* session_;
	session* session_ptr_;
	socket_stream* stream_;
	bool first_;
	char local_charset_[32];
	int  rw_timeout_;
	bool parse_body_enable_;
	int  parse_body_limit_;

	void init();
};

} // namespace acl
