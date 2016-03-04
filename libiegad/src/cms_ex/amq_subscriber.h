#ifndef __AMQ_SUBSCRIBER__
#define __AMQ_SUBSCRIBER__



// ============ ˵�� ============
//
// @�������� : 2016-02-21
// @������ : iegad
//
// ============================
// @��; : 1, ��ActiveMQ-CMS ����չ, ��װһ�� "������" ���㿪��ʹ��.
//		
// @PS : ���ļ������� ActiveMQ-CMS ��Դ��
//
// @bugs : Ŀǰ "����-����"ģʽ ��֧�־û�, �ؼ�������� ������.
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================



#include "amq_baser.h"
#include "amq_iworker.hpp"



namespace iegad {
namespace cms_ex {


    class AMQ_Subscriber : public AMQ_Baser, public AMQ_IRecver {
	//AMQ ������, ������ AMQ_Baser, ���̳�AMQ_IRecver �ӿ�
    public:
	// ============================
	// @��; : ���캯��
	// @PS : �ṩĬ�Ϲ����Ŀ���ǿ��Խ�� ����ģ�� ��ʹ��.
	// ============================
	AMQ_Subscriber() {}
    

	// ============================
	// @��; : ��������
	// ============================
	virtual ~AMQ_Subscriber() {
	    this->_cleanup();
	}


	void SetClientId(const std::string & clientId) {
	    this->client_id_ = clientId;
	}


	// ============================
	// @��; : �����ַ�����Ϣ, �̳��� AMQ_IRecver
	// @txtstr : �ַ�����Ϣ : out����
	// @errstr : ������Ϣ : out����
	// @����ֵ : �ɹ����շ��� true, ���򷵻� false	    
	// ============================
	virtual bool RecvTextMessage(std::string * txtstr, std::string * errstr = nullptr);


	// ============================
	// @��; : ���ն�������Ϣ, �̳��� AMQ_IRecver
	// @txtstr : �ַ�����Ϣ : out����
	// @errstr : ������Ϣ : out����
	// @����ֵ : �ɹ����շ��� true, ���򷵻� false	    
	// ============================
	virtual bool RecvBytesMessage(std::string * buffstr, unsigned int * buffsize, std::string * errstr = nullptr);


    private:
	// ============================
	// @��; : ��ʼ���ڲ��ֶ�ֵ
	// @����ֵ : void
	// @PS : ��չ���ຯ��
	// ============================
	virtual void _init();


	// ============================
	// @��; : �ͷ���ռ�õ�ϵͳ��Դ
	// @����ֵ : void
	// @PS : ��չ���ຯ��
	// ============================
	virtual void _cleanup();


	// �����߶���ָ��
	std::shared_ptr<::cms::MessageConsumer> subscriber_;

	std::string client_id_;
    }; // AMQ_Subscriber;


} // namespace cms_ex;
} // namespace iegad;


#endif // __AMQ_SUBSCRIBER__