#ifndef __AMQ_PUBLISHER__
#define __AMQ_PUBLISHER__



// ============ ˵�� ============
//
// @�������� : 2016-02-22
// @������ : iegad
//
// ============================
// @��; : 1, ��ActiveMQ-CMS ����չ, 
//
//		    ��װһ�� "������" ���㿪��ʹ��.
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


    class AMQ_Publisher : public AMQ_Baser, public AMQ_ISender {
    //AMQ ������ : ������ AMQ_Baser, ���̳� AMQ_ISender �ӿ�
    public:
	// ============================
	// @��; : ���캯��
	// @PS : �ṩĬ�Ϲ����Ŀ���ǿ��Խ�� ����ģ�� ��ʹ��.
	// ============================
	AMQ_Publisher() {}


	// ============================
	// @��; : ��������
	// ============================
	virtual ~AMQ_Publisher() { 
	    this->_cleanup(); 
	}


	void SetClientId(const std::string & clientId) {
	    this->client_id_ = clientId;
	}


	// ============================
	// @��; : ���� string ������Ϣ, �̳��� AMQ_ISender
	// @serstr : ��Ҫ���͵��ַ���
	// @errstr : ������Ϣ : out����
	// @����ֵ : ���ͳɹ����� true, ���򷵻� false.
	// ============================
	virtual bool SendTextMessage(const std::string & txtstr, std::string * errstr = nullptr) override;


	// ============================
	// @��; : ���� ���������� ������Ϣ, �̳��� AMQ_ISender
	// @buff : ���ݻ�����ָ��
	// @buffSize : ���ݻ�������С
	// @errstr : ������Ϣ : out����
	// @����ֵ : ���ͳɹ����� true, ���򷵻� false.
	// ============================
	virtual bool SendBytesMessage(const char * buff, unsigned int buffsize, std::string * errstr = nullptr) override;


    private:
	// ============================
	// @��; : ��ʼ���ڲ��ֶ�ֵ
	// @����ֵ : void
	// ============================
	virtual void _init() override;


	// ============================
	// @��; : �ͷ���ռ�õ�ϵͳ��Դ
	// @����ֵ : void
	// ============================
	virtual void _cleanup() override;


	// �����߶���ָ��
	std::shared_ptr<::cms::MessageProducer> publisher_;

	std::string client_id_;
    }; // class AMQ_Publisher;


} // namespace cms_ex;
} // namespace iegad;



#endif // __AMQ_PUBLISHER__