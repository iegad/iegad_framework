#ifndef __BASIC_SERVICE__
#define __BASIC_SERVICE__



// ============ ˵�� ============
//
// @�������� : 2015-10-01
// @������ : iegad
//
// ============================
// @��; : �÷������, �����з��������Ļ���, 
//		1. ͳһ���ýӿ�
//		2. �ṩһЩ�����ĺ���
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================
//  -- 2015-10-10	--iegad			1. Ϊ_response ������ܹ���
//  -- 2015-10-22    --iegad			1. �� _response ���� Ϊ _return
//  -- 2015-11-08    --iegad			1, �滻 �°汾 msg ���պͷ��ͺ���



#include <string>
#include <memory>
#include "msg/iegad_msg.h"


namespace iegad {
namespace nets {

    
    class basic_svc {
    // ����������, ������;

    public:
	// ============================
	// @��; : �౾�������ָ�� ��������
	// ============================
	typedef std::shared_ptr<basic_svc> basic_svc_ptr;


	// ============================
	// @��; : ���ظö���ķ���ID
	// @����ֵ : ���ط���ID
	// ============================
	int get_id();


	// ============================
	// @��; : ִ�� tcp�ͻ��� clnt �����͵��������;
	// @clnt : ��������tcp�ͻ���;
	// @msg_flag : ��Ϣ��־ msg_baisc::msg_flag
	// @msg_bdstr : ����Ϣ�Ĺ����ַ���
	// @����ֵ : ���óɹ����� 0, ���򷵻� -1; 
	// @PS : �������ĵ��ýӿ�, �ú���Ϊ���麯��, ��ʵ�ʵķ������(������)ʵ��;
	// ============================
	virtual int invoke(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr) = 0;


    protected:
	// ============================
	// @��; : ���캯��
	// @svc_id : ����ID
	// ============================
	explicit basic_svc(int svc_id);


	// ============================
	// @��; : ��������
	// ============================
	virtual ~basic_svc() {}


	// ============================
	// @��; : ͨ�� ����Ϣ�����ַ��� msgbdstr ������ ����Ϣ msg;
	// @msgbdstr : ����Ϣ�Ĺ����ַ���
	// @msg : ����Ϣ����
	// @����ֵ, �ɹ���������0, ����, ����-1;
	// @PS : ģ����� __MSG_T һ��Ҫ����google protocol buffer
	//		�����ɵ�msg��������.
	// ============================
	template <class __MSG_T> 
	int _build_svc(const std::string & msgbdstr, __MSG_T & msg);


	// ============================
	// @��; : ��tcp�ͻ��� clnt ���� ��Ϣ msg;
	// @clnt : tcp �ͻ���
	// @flag : ��Ϣ��־, basic_msg::msg_flag
	// @msg : ����Ϣ����;
	// @����ֵ, ���ͳɹ�����0, ����, ����-1;
	// @PS : ģ����� __MSG_T һ��Ҫ����google protocol buffer
	//		�����ɵ�msg��������.
	// ============================
	template <class __MSG_T>
	int _send_msg(boost::asio::ip::tcp::socket & clnt, int flag, const __MSG_T & msg);


	// ============================
	// @��; : ��tcp�ͻ��� clnt ����Ӧ��;;
	// @clnt : tcp �ͻ���
	// @rzt : Ӧ��Ľ��, �������Լ�����
	// @rzt_size : Ӧ�����ݵĳ���
	// @err_code : ������Ϣ
	// @����ֵ, ���ͳɹ����͵��ֽ���
	// @PS : Ϊ�����ͨ�ŴӶ���Ӹú���, ���������
	//		ÿ��Ӧ��ʱ, ������һ��basic_msg����;
	// ============================
	int _return(boost::asio::ip::tcp::socket & clnt, const char * rzt, size_t rzt_size, boost::system::error_code & err_code);


    private:
	// ����ID
	int svc_id_;
    }; // class basic_svc



// ==================  ����Ϊʵ�ֲ��� ==================


    template <class __MSG_T>
    int iegad::nets::basic_svc::_build_svc(const std::string & msgbdstr, __MSG_T & msg)
    {
        msg.Clear();
        return msg.ParseFromString(msgbdstr) ? 0 : -1;
    }


    template <class __MSG_T>
    int iegad::nets::basic_svc::_send_msg(boost::asio::ip::tcp::socket & clnt, int flag, const __MSG_T & msg)
    {
        std::string msg_str;
        boost::system::error_code errcode;

        if (!msg.SerializeToString(&msg_str)) {
            return -1;
        }

        iegad::msg::basic_msg msgbsc;
	std::string msgstr;
        msgbsc.set_msg_type(this->svc_id_);
        msgbsc.set_msg_flag(flag);
        msgbsc.set_msg_bdstr(msg_str);
	if (msgbsc.SerializeToString(&msgstr)) {
	    return iegad::msg::send_str(clnt, msgstr, errcode) == msgstr.size() + 1 ? 0 : -1;
	}
	return -1;
    }


} // namespace nets;
} // namespace iegad;


#endif // __BASIC_SERVICE__
