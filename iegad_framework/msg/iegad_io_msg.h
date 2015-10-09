#ifndef __IEGAD_IO_MSG__
#define __IEGAD_IO_MSG__



// ============ ˵�� ============
//
// @�������� : 2015-09-27
// @������ : iegad
//
// ============================
// @��; : 1. ���� boost::asio �ķ�װ���ṩһ�� �򵥵�IO����.
//		 2. �� basic_msg �Ĳ�����װ.
// ============================
//
// @�޸ļ�¼:
// =======================================
//	    ����          |      �޸���         |                         �޸�˵��
// =======================================
//  --2015-10-02	--iegad		-- �� io������������err_code �ش���������
//  --2015-10-09	--iegad		-- ע�͵� send_n ����, ����boost::asio �Դ��� send ����, ��������



#ifdef WIN32

#pragma comment(lib, "libprotoc.lib")
#pragma comment(lib, "libprotobuf.lib")

#endif // WIN32


#include <boost/asio.hpp>
#include <string>
#include "msg/basic_msg.pb.h"



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
    const std::string recv_end(boost::asio::ip::tcp::socket & clnt, 
	boost::system::error_code & err_code);


 //   // ============================
 //   // @��; : �� �ͻ���clnt ���� send_buf ֱ������buf_size����Ϊֹ;
 //   // @clnt : tcp �ͻ���
 //   // @send_buf : �����͵�����
 //   // @buf_size, ���������ݵĴ�С
 //   // @err_code : �ش��������ߵ� err_code
 //   // @����ֵ : ���ط��͵��ֽ���
 //   // ============================
 //   int send_n(boost::asio::ip::tcp::socket & clnt, const char * send_buf, int buf_size, 
	//boost::system::error_code & err_code);


} // namespace net;


namespace net {


    // ============================
    // @��; : �� �ͻ���clnt ��ȡ��Ϣ msgbsc;
    // @clnt : tcp �ͻ���
    // @msgbsk : ��Ҫ������Ϣ
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : ���ճɹ����� 0 , ���򷵻� -1;
    // ============================
    int recv_basic_msg(boost::asio::ip::tcp::socket & clnt, basic_msg & msgbsc, 
	boost::system::error_code & err_code);


    // ============================
    // @��; : �� �ͻ���clnt ������Ϣ msgbsc;
    // @clnt : tcp �ͻ���
    // @msgbsc : ��Ҫ���͵� basic_msg
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : �ɹ����ͷ��� 0 , ���򷵻� -1;
    // ============================
    int send_basic_msg(boost::asio::ip::tcp::socket & clnt, const basic_msg & msgbsc, 
	boost::system::error_code & err_code);


    // ============================
    // @��; : �� �ͻ���clnt ������Ϣ basic_msg,
    //		    ��basic_msg����type, flag, msg_str�����;
    // @clnt : tcp �ͻ���
    // @type : basic_msg.msg_type
    // @flag : basic_msg.msg_flag
    // @msg_bdstr : basic_msg.msg_bdstr
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : �ɹ����ͷ��� 0 , ���򷵻� -1;
    // ============================
    int send_basic_msg(boost::asio::ip::tcp::socket & clnt, int type, int flag, const std::string & msg_bdstr, 
	boost::system::error_code & err_code);


} // namespace net;



} // namespace iegad;



#endif // __IEGAD_IO_MSG__