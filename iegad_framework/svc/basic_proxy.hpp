#ifndef __BASIC_PROXY__
#define __BASIC_PROXY__


// ============ ˵�� ============
//
// @�������� : 2015-10-04
// @������ : iegad
//
// ============================
// @��; :  ����Ϊ�����, 
//		  �������ṩ�ͻ��˵��� ������� �Ĵ���, ʵ������һ��ͳһ���õĽӿ�.
//		  �����ʵ��, ʹ�� ���� operator() �����,
//		  ����ʹ������������һ����������, �������� �ͻ��˴������
// @PS : �ͻ����ڽ��� �ͻ��˴��� �� ������� ��ͨ��ʱ, Ӧ���ڸ���
//		������һ���ʺ��Լ��ķ����������Ӧ�Ĵ�����
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================
// -- 2015-10-09	    --iegad				-- Ϊ���������, ���� ��pb��Ϣ���� ����ȥ��, �����Զ�������л�����ʽ,
//									    ȥ�� ��pb����, ֱ�Ӵ����ַ���, �����ṩ���ϲ����Ŀ���, �Ǵ����ı���Ϣ, 
//									    ���Ǵ���һ����������л�. ͬʱҲ������һ�����л��Ĺ���.


#include <boost/asio.hpp>
#include "msg/iegad_io_msg.h"


namespace iegad {
namespace net {


    // ============================
    // @__R :  operator() ����ֵ����
    // @__P : operator() ��������
    // @__MSG_T : pb�����ɵ���Ϣ����
    // ============================
    template <typename __R, typename __P>
    class basic_proxy {
    // �ͻ��������
    public:


	// ============================
	// @��; : ���캯��
	// @host : ����˻�������IP��ַ
	// @svc : ��������˿ں�
	// @PS : ��������, ���host ����Ϊ�������, ����ʱ, ��ռ�ñȽϳ���ʱ��
	//		����, host ���������ܵ�ʹ�� ip ��ַ.
	// ============================
	explicit basic_proxy(const std::string & host, const std::string & svc)
	    : ios_(), clnt_(ios_), host_(host), svc_(svc) {}


	// ============================
	// @��; : ��������
	// @PS : �ú���Ϊ�ر� clnt_ ���׽��ֶ���
	// ============================
	virtual ~basic_proxy();


	// ============================
	// @��; : Զ�̷��������ýӿ�
	// @param : ���ò���, �����Ҫ���ݶ������, ��Ҫ����һ���ṹ��������
	// @����ֵ : ģ��� __R ����, ����ֵ, �����������ж������.
	// ============================
	virtual const __R operator()(const __P & param) = 0;

    protected:
	// ============================
	// @��; : Զ�̷��������ýӿ�
	// @param : ���ò���, �����Ҫ���ݶ������, ��Ҫ����һ���ṹ��������
	// @����ֵ : �ɹ����ͷ���0, ���򷵻� -1.
	// ============================
	int _send_msg(int msg_type, int msg_flag, const std::string & msg_dbstr);


	// ============================
	// @��; : ������Ӧ����Ϣ, �����һ�� basic_msg ���Ͷ���
	// @msgbsc : �������յ�basic_msg ����
	// @err_code : ��������ʱ, �������մ�����Ϣ
	// @����ֵ : �ɹ�����0, ���򷵻� -1.
	// ============================
	int _recv_msg(basic_msg & msgbsc, boost::system::error_code & err_code);


	// ============================
	// @��; : ������Ӧ����Ϣ, ��Ϣ����Ϊ�ַ�����ʽ�Ļ�������, 
	//		����ͨ��ת������, ������ֵת����Ԥ�ڵ�����
	// @����ֵ : �ɹ����ض�Ӧ���͵��ַ�����ʽ, ����, ���� "" һ�����ַ���.
	// ============================
	const std::string _recv();


	// ============================
	// @��; : ���ӷ���˽���
	// @����ֵ : �ɹ�����, ����0, ����, ���� -1
	// ============================
	int _connect();


	// boost::asio::io_service ����
	boost::asio::io_service ios_;
	// boost�׽��ֶ���
	boost::asio::ip::tcp::socket clnt_;

    private:
	// ����˻�����, ��IP
	std::string host_;
	// ��������˿�
	std::string svc_;


	// ����
	basic_proxy(const basic_proxy &);
	basic_proxy & operator=(const basic_proxy &);

    }; // class basic_proxy<__R, __P, __MSG_T>;



// ============================ ������ʵ�ֲ��� ============================



    template <typename __R, typename __P>
    iegad::net::basic_proxy<__R, __P>::~basic_proxy()
    {
	if (clnt_.is_open()) {
	    clnt_.close();
	}
    }


    template <typename __R, typename __P>
    int iegad::net::basic_proxy<__R, __P>::_send_msg(int msg_type, int msg_flag, const std::string & msg_bdstr)
    {
	boost::system::error_code err_code;
	int n = iegad::net::send_basic_msg(clnt_, msg_type, msg_flag, msg_bdstr, err_code);
	if (n == 0) {
	    clnt_.shutdown(boost::asio::socket_base::shutdown_send);
	}
	return n;
    }


    template <typename __R, typename __P>
    int iegad::net::basic_proxy<__R, __P>::_recv_msg(basic_msg & msgbsc, boost::system::error_code & err_code)
    {
	return iegad::net::recv_basic_msg(clnt_, msgbsc, err_code);
    }


    template <typename __R, typename __P>
    const std::string iegad::net::basic_proxy<__R, __P>::_recv()
    {
	boost::system::error_code err_code;
	char buff[BUF_SIZE];
	std::string res;
	int n;
	do {
	    n = clnt_.read_some(boost::asio::buffer(buff), err_code);
	    if (err_code) {
		if (err_code == boost::asio::error::eof) {
		    break;
		}
		else if (err_code == boost::asio::error::interrupted){
		    continue;
		}
		else {
		    res = "";
		    break;
		}
	    }
	    res.append(buff, n);
	} while (true);
	return iegad::string::de_cust(res, MSG_KEY);
    }


    template <typename __R, typename __P>
    int iegad::net::basic_proxy<__R, __P>::_connect()
    {
	boost::asio::ip::tcp::resolver::iterator end;
	boost::system::error_code errcode = boost::asio::error::host_not_found;


	boost::asio::ip::tcp::resolver rlv(clnt_.get_io_service());
	boost::asio::ip::tcp::resolver::query qry(host_, svc_);

	boost::asio::ip::tcp::resolver::iterator iter = rlv.resolve(qry);
	for (; errcode && iter != end; ++iter) {
	    clnt_.close();
	    if (clnt_.connect(*iter, errcode) == 0) {
		clnt_.set_option(boost::asio::ip::tcp::no_delay(true));
		return 0;
	    }
	} // for (; errcode && iter != end; ++iter);

	if (errcode) {
	    iWARN << errcode.message() << std::endl;
	}
	return -1;
    }



} // namespace net;
} // namespace iegad;



#endif // __BASIC_PROXY__