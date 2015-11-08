#ifndef __TCP_MT_SVR__
#define __TCP_MT_SVR__



// ============ ˵�� ============
//
// @�������� : 2015-10-02
// @������ : iegad
//
// ============================
// @��; : ���̲߳���������
//	1. ���ײ�ͨ��, ��Ϣ���ս���
//	    ��װ, �ﵽ Ӧ�ò� �� ��Ϣ�շ��� �ķ���.
//	2. ʹ�� ׷����-�쵼�� ģ��;
// @PS : ��ʵ��ʱӦ�ò����˽���ʱ, ��󽫸���
//		��Ϊ����;
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================
// -- 2015-10-02	    -- iegad		--1. Ϊ�ͻ��� ���ݽ��� �趨��ʱֵ, �ѷ�ֹע�빥��
//								--2. ���� io ������������ err_code;
//  --2015-10-10	    -- iegad		--  �޸Ĺ��캯��, ȥ��host ����, 
//								    ʹ�� 0.0.0.0 �ķ�ʽ���󶨼����׽���
//  --2015-10-10	    -- iegad		--  ��� tcp_mt_svr (const std::string &, const std::string &)
//								    ���캯��, ���ṩ ������� �ͷ����� ��ʽ��
//  --2015-11-08	    -- iegad		--1, ȥ���� protocbuff ������.
//								    2, �� _call_svc ��Ϊ _action ���趨Ϊ ���麯��.
//								    3, �� service ������ server�������, ȥ�� regist_svc
//								    4, ��ʱʱ���Ϊ 10 ��
//								    5, �� msg �� ��/������ �滻Ϊ�°汾


#include <boost/asio.hpp>


#include <mutex>
#include <vector>
#include <thread>
#include <functional>


#include "nets/basic_svc.h"


namespace iegad {
namespace nets {


    using namespace boost::asio;


    class tcp_mt_svr {
    // ���̲߳���������

    public:
	// ============================
	// @��; : �̳߳� ��������
	// ============================
	typedef std::vector<std::thread> thread_pool_t;


	// ============================
	// @��; : �̻߳����� ��������
	// ============================
	typedef std::unique_lock<std::mutex> mtx_lock_t;	


	// ============================
	// @��; : ���캯��
	// @port : �˿ں�
	// ============================
	explicit tcp_mt_svr(unsigned short port);


	// ============================
	// @��; : ���캯��
	// @host : ��������IP��ַ
	// @svc : �˿ںŻ������
	// ============================
	tcp_mt_svr(const std::string & host, const std::string & svc);


	// ============================
	// @��; : ��������
	// @PS : ��Ϊ���ܱ���������, ������������Ϊ�麯��
	// ============================
	virtual ~tcp_mt_svr();


	// ============================
	// @��; : ��ʼ �ͻ�����Ϣ�����߳�
	// @n : �߳�����.
	// @����ֵ : void
	// ============================
	void run(int n = 8);


	// ============================
	// @��; : ֹͣ �ͻ�����Ϣ�����߳�.
	// @����ֵ : void
	// ============================
	void stop();


	// ============================
	// @��; : ���ؼ����׽���, ���󶨵ĵ�ַ
	// @����ֵ : ����� IP ��ַ
	// ============================
	const std::string host_endpoint();


    protected:
	// ============================
	// @��; : ������Ƿ�ֹͣ
	// @����ֵ : ֹͣ���� true, ���򷵻� false; 
	// ============================
	bool _is_stop();


	// ============================
	// @��; : �������.
	// @clnt : �������Ŀͻ���
	// @msgbsc : �������� Ӧ�÷������ ��basic_msg����.
	// @����ֵ : �ɹ����ͷ������ ���� 0, ���򷵻� -1; 
	// @PS : ���麯��, ����������д
	// ============================
	virtual int _action(ip::tcp::socket & clnt, const std::string & msgstr) = 0;


    private:
	// ============================
	// @��; : �ͻ�����Ϣ�����߳�
	// @����ֵ : ֹͣ���� true, ���򷵻� false; 
	// ============================
	void _thread_proc();


	// ============================
	// @��; : �ȴ��ͻ�������, �ɹ����Ӻ�, ���ͻ��˻Ự������ clnt;
	// @clnt : �����յĿͻ��˶���
	// @err_code : �����ʹ���ʱ, �������մ���
	// @����ֵ : �ͻ��˳ɹ��������ӷ��� 0, ���򷵻� -1; 
	// ============================
	int _accept(ip::tcp::socket & clnt, 
	    boost::system::error_code & err_code);


	// ============================
	// @��; : ���տͻ��� clnt ���� �ַ�����Ϣ
	// @clnt : �ͻ���
	// @recvbuff : boost::asio::streambuf 
	// @err_code : ������Ϣ
	// @����ֵ : ���ճɹ� ���� �ͻ��������͵��ַ���, ���򷵻� ERR_STRING; 
	// @PS : ���ͻ��˷��ͳ�ʱ, ͬ������ ERR_STRING
	// ============================
	const std::string _get_msgstr(ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff, 
	    boost::system::error_code & err_code);


	// ֹͣ��־
	bool stop_flag_;
	// �ͻ�����Ϣ�����߳���
	std::mutex thread_mtx_;
	// ֹͣ��־��
	std::mutex stop_mtx_;
	// ��������� boost::io_service
	io_service ios_;
	// �ͻ�����Ϣ�����߳� �̳߳�
	thread_pool_t thread_pool_;
	// ��������
	ip::tcp::acceptor acptor_;
	// ��ʱֵ 
#ifdef WIN32 // for win
	const int timeout_ = 10000;
#else // for unix
    const timeval timeout_ = {10, 0};
#endif // WIN32


	// ����
	tcp_mt_svr(const tcp_mt_svr &);
	tcp_mt_svr & operator=(const tcp_mt_svr &);
    };

} // namespace nets;
} // namespace iegad


#endif // __TCP_MT_SVR__
