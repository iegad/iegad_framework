#ifndef __IEGAD_IO_MSG__
#define __IEGAD_IO_MSG__



// ============ ˵�� ============
//
// @�������� : 2015-09-27
// @������ : iegad
//
// ============================
// @��; : 1. ���� boost::asio �ķ�װ���ṩһ�� �򵥵�IO����.
//		 2. �� msg_basic �Ĳ�����װ.
// ============================
//
// @�޸ļ�¼:
// =======================================
//	    ����          |      �޸���         |                         �޸�˵��
// =======================================
//  --2015-10-02	--iegad		-- �� io������������err_code �ش���������



#ifdef WIN32

#pragma comment(lib, "libprotoc.lib")
#pragma comment(lib, "libprotobuf.lib")

#endif // WIN32


#include <boost/asio.hpp>
#include <string>
#include "msg_basic.pb.h"



namespace iegad {
namespace io {


    // �ͻ��˽��ջ����С
    enum {
	BUF_SIZE = 1024 * 4
    };
     

    // ============================
    // @��; : �� �ͻ���clnt ������ֱ��eofΪֹ;
    // @clnt : tcp �ͻ���
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : ���ض�ȡ�Ļ���������
    // ============================
    const std::string recv_end(boost::asio::ip::tcp::socket & clnt, boost::system::error_code & err_code);


    // ============================
    // @��; : �� �ͻ���clnt ���� send_buf ֱ������buf_size����Ϊֹ;
    // @clnt : tcp �ͻ���
    // @send_buf : �����͵�����
    // @buf_size, ���������ݵĴ�С
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : ���ط��͵��ֽ���
    // ============================
    int send_n(boost::asio::ip::tcp::socket & clnt, const char * send_buf, int buf_size, boost::system::error_code & err_code);


} // namespace net;


namespace net {


    // ============================
    // @��; : �� �ͻ���clnt ��ȡ��Ϣ msgbsc;
    // @clnt : tcp �ͻ���
    // @msgbsk : ��Ҫ������Ϣ
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : ���ճɹ����� 0 , ���򷵻� -1;
    // @PS : ������ timeout ��Ϊ 0, ���Ϊclnt �趨���ճ�ʱ, 
    //		 ����, ���ս���������.
    // ============================
    int recv_msg_basic(boost::asio::ip::tcp::socket & clnt, msg_basic & msgbsc, boost::system::error_code & err_code);


    // ============================
    // @��; : �� �ͻ���clnt ������Ϣ msgbsc;
    // @clnt : tcp �ͻ���
    // @msgbsc : ��Ҫ���͵� msg_basic
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : �ɹ����ͷ��� 0 , ���򷵻� -1;
    // ============================
    int send_msg_basic(boost::asio::ip::tcp::socket & clnt, const msg_basic & msgbsc, boost::system::error_code & err_code);


    // ============================
    // @��; : �� �ͻ���clnt ������Ϣ msg_basic,
    //		    ��msg_basic����type, flag, msg_str�����;
    // @clnt : tcp �ͻ���
    // @type : msg_basic.msg_type
    // @flag : msg_basic.msg_flag
    // @msg_bdstr : msg_basic.msg_bdstr
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : �ɹ����ͷ��� 0 , ���򷵻� -1;
    // ============================
    int send_msg_basic(boost::asio::ip::tcp::socket & clnt, int type, int flag, const std::string & msg_bdstr, boost::system::error_code & err_code);


} // namespace net;



} // namespace iegad;



#endif // __IEGAD_IO_MSG__