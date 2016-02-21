#ifndef __AMQ_IWORKER__
#define __AMQ_IWORKER__



// ============ ˵�� ============
//
// @�������� : 2016-02-21
// @������ : iegad
//
// ============================
// @��; : 1, ��ActiveMQ-CMS ����չ, �ṩ ������ �� ������ �ӿ�Լ��
//		
// @PS : ���ļ������� ActiveMQ-CMS ��Դ��
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================



#include <string>



namespace iegad {
namespace cms_ex {


    class AMQ_ISender {
    // ������ �ӿ�
    public:
	virtual bool SendTextMessage(const std::string & txtstr, std::string * errstr = nullptr) = 0;
	virtual bool SendBytesMessage(const char * buff, unsigned int buffsize, std::string * errstr = nullptr) = 0;
    }; // class AMQ_ISender;


    class AMQ_IRecver {
    // // ������ �ӿ�
	virtual bool RecvTextMessage(std::string * txtstr, std::string * errstr = nullptr) = 0;
	virtual bool RecvBytesMessage(std::string * buffstr, unsigned int * buffsize, std::string * errstr = nullptr) = 0;
    }; // class AMQ_IRecver;


} // namespace cms_ex;
} // namespace iegad;


#endif // __AMQ_IWORKER__