#ifndef __IEGAD_MSG__
#define __IEGAD_MSG__



// ============ ˵�� ============
//
// @�������� : 2015-09-27
// @������ : iegad
//
// ============================
// @��; : 1. ���� boost::asio �ķ�װ���ṩһ�� �򵥵�IO����.
//		 2. �� msg �Ĳ�����װ.
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



#ifdef WIN32

#pragma comment(lib, "libprotoc.lib")
#pragma comment(lib, "libprotobuf.lib")

#endif // WIN32


#include <boost/asio.hpp>
#include <string>
#include "iegad_define.h"


namespace iegad {
namespace msg {


    // ============================
    // @��; : �� �ͻ���clnt ��ȡ �ַ�����Ϣ;
    // @clnt : tcp �ͻ���
    // @recvbuff : boost::asio::streambuf ������
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : ���ճɹ����� �ͻ��˴��͵��ַ�����Ϣ , ���򷵻� ERR_STRING;
    // ============================
    const std::string recv_str(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff, 
	boost::system::error_code & err_code);

    // ============================
    // @��; : recv_str => ���ܰ�
    // ============================
    const std::string recv_str(boost::asio::ip::tcp::socket & clnt, boost::asio::streambuf & recvbuff, 
	boost::system::error_code & err_code, char msg_key);


    // ============================
    // @��; : �� �ͻ���clnt ���� �ַ�����Ϣ;
    // @clnt : tcp �ͻ���
    // @msgstr : ��Ҫ���͵� �ַ�����Ϣ;
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : �ɹ����ͷ��� �ַ�����Ϣ����, ���򷵻� -1 ���� ����err_code;
    // ============================
    int send_str(boost::asio::ip::tcp::socket & clnt, const std::string & msgstr, 
	boost::system::error_code & err_code);

    // ============================
    // @��; : send_str => ���ܰ�
    // ============================
    int send_str(boost::asio::ip::tcp::socket & clnt, const std::string & msgstr, 
	boost::system::error_code & err_code, char msg_key);


    // ============================
    // @��; : ���� �ͻ���clnt ���͵� �ļ�, ��д�� ָ���� filename�ļ�
    // @clnt : tcp �ͻ���
    // @filename : �ļ�����·�� + �ļ���
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : �ɹ����ͷ��� ���յ��ļ����� , ���򷵻� -1, ����err_code;
    // ============================
    int recv_file(boost::asio::ip::tcp::socket & clnt, const std::string & filename, boost::system::error_code & err_code);


    // ============================
    // @��; : �� �ͻ���clnt ���͵� �ļ�
    // @clnt : tcp �ͻ���
    // @filename : �ļ�����·�� + �ļ���
    // @err_code : �ش��������ߵ� err_code
    // @����ֵ : �ɹ����ͷ��� ���͵��ļ����� , ����, ���� -1, ����err_code;
    // @PS : ���������ļ�֮��, ����˻�ر� ���� ͨ��
    // ============================
    int send_file(boost::asio::ip::tcp::socket & clnt, const std::string & filename, boost::system::error_code & err_code);


} // namespace msg;
} // namespace iegad;



#endif // __IEGAD_MSG__