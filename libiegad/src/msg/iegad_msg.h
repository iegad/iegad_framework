#ifndef __IEGAD_MSG__
#define __IEGAD_MSG__



// ============ ˵�� ============
//
// @�������� : 2015-09-27
// @������ : iegad
//
// ============================
// @��; : 1. ���� boost::asio �ķ�װ ���ṩ�� "�ַ���" �� "������" ��Ϣ �շ�����.
// ============================
//
// @�޸ļ�¼:
// =======================================
//	    ����          |      �޸���         |                         �޸�˵��
// =======================================
//  --2015-10-02	--iegad		-- �� io������������err_code �ش���������;
//  --2015-10-09	--iegad		-- ע�͵� send_n ����, ����boost::asio �Դ��� send ����, ��������;
//  --2015-10-23	--iegad		-- �� net �����ռ� ��Ϊ msg �����ռ�;
//  --2015-11-07     --iegad		-- 1, ȥ�� google protocol buffer ������;
//							    2, ȥ��protobuf �� basic_msg ����;
//							    3, ��� ������ ��Ϣ���� bmsg_t;
//							    4, ��� �ַ�����Ϣ���ļ���Ϣ ��recv/send ��������.
//  --2015-11-08	--iegad		-- ��� ���� ����/���� ����


#include <boost/asio.hpp>
#include <string>
#include "iegad_define.h"


namespace iegad {
namespace msg {


    // ============================
    // @��; : �� �ͻ���clnt ��ȡ �ַ�����Ϣ;
    // @clnt : tcp �ͻ���
    // @recvbuff : boost::asio::streambuf ������
    // @err_code : ����Ϣ��
    // @����ֵ : ���ճɹ����� �ַ�����Ϣ , ���򷵻� ERR_STRING, ����err_code;
    // ============================
    const std::string recv_str(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff, 
	boost::system::error_code & err_code);

    // ============================
    // @��; : recv_str => ���ܰ�
    // @msg_key : ��Կ
    // ============================
    const std::string recv_str(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff, 
	boost::system::error_code & err_code, char msg_key);


    // ============================
    // @��; : �� �ͻ���clnt ���� �ַ�����Ϣ;
    // @clnt : tcp �ͻ���
    // @msgstr : �ַ�����Ϣ;
    // @err_code : ����Ϣ��
    // @����ֵ : �ɹ����ͷ��� �ַ�����Ϣ����, ���򷵻� -1 ���� ����err_code;
    // ============================
    int send_str(boost::asio::ip::tcp::socket & clnt, const std::string & msgstr, 
	boost::system::error_code & err_code);

    // ============================
    // @��; : send_str => ���ܰ�
    // @msg_key : ��Կ
    // ============================
    int send_str(boost::asio::ip::tcp::socket & clnt, const std::string & msgstr, 
	boost::system::error_code & err_code, char msg_key);


    // ============================
    // @��; : ���� �ϴ��ļ�, ���Ϊ filename�ļ�
    // @clnt : �ͻ���
    // @filename : �ļ�����·�� + �ļ���
    // @err_code : ����Ϣ��
    // @����ֵ : �ɹ������ļ����� �������ļ���С , ���򷵻� -1, ����err_code;
    // ============================
    int recv_file(boost::asio::ip::tcp::socket & clnt, const std::string & filename, boost::system::error_code & err_code);


    // ============================
    // @��; : ��ͻ��˷��͵��ļ�
    // @clnt : �ͻ���
    // @filename : �ļ�����·�� + �ļ���
    // @err_code : ����Ϣ��
    // @����ֵ : �ɹ������ļ����� �������ļ���С , ����, ���� -1, ����err_code;
    // @PS : ���������ļ�֮��, ����˻�ر� ���� ͨ��
    // ============================
    int send_file(boost::asio::ip::tcp::socket & clnt, const std::string & filename, boost::system::error_code & err_code);


} // namespace msg;
} // namespace iegad;



#endif // __IEGAD_MSG__