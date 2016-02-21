#ifndef __AMQ_CONSUMER__
#define __AMQ_CONSUMER__



// ============ ˵�� ============
//
// @�������� : 2016-02-21
// @������ : iegad
//
// ============================
// @��; : 1, ��ActiveMQ-CMS ����չ, ��װһ�� "������" ���㿪��ʹ��.
//		
// @PS : ���ļ������� ActiveMQ-CMS ��Դ��
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


	class AMQ_Consumer : public AMQ_Baser, public AMQ_IRecver {
	//AMQ ������, ������ AMQ_Baser, ���̳�AMQ_IRecver �ӿ�
	public:
	    // ============================
	    // @��; : ���캯��
	    // @PS : �ṩĬ�Ϲ����Ŀ���ǿ��Խ�� ����ģ�� ��ʹ��.
	    // ============================
	    AMQ_Consumer() {}


	    // ============================
	    // @��; : ��������
	    // ============================
	    virtual ~AMQ_Consumer() { this->_cleanup(); }


	    // ============================
	    // @��; : �����ַ�����Ϣ, �̳��� AMQ_IRecver
	    // @txtstr : �ַ�����Ϣ : out����
	    // @errstr : ������Ϣ : out����
	    // @����ֵ : �ɹ����շ��� true, ���򷵻� false	    
	    // ============================
	    virtual bool RecvTextMessage(std::string * txtstr, std::string * errstr = nullptr) override;


	    // ============================
	    // @��; : ���ն�������Ϣ, �̳��� AMQ_IRecver
	    // @txtstr : �ַ�����Ϣ : out����
	    // @errstr : ������Ϣ : out����
	    // @����ֵ : �ɹ����շ��� true, ���򷵻� false	    
	    // ============================
	    virtual bool RecvBytesMessage(std::string * buffstr, unsigned int * buffsize, std::string * errstr = nullptr) override;


	private:
	    // ============================
	    // @��; : ��ʼ���ڲ��ֶ�ֵ
	    // @����ֵ : void
	    // @PS : ��չ���ຯ��
	    // ============================
	    virtual void _init() override;


	    // ============================
	    // @��; : �ͷ���ռ�õ�ϵͳ��Դ
	    // @����ֵ : void
	    // @PS : ��չ���ຯ��
	    // ============================
	    virtual void _cleanup() override;


	    // ������ָ��
	    std::shared_ptr<::cms::MessageConsumer> consumer_;	    
	}; // class AMQ_Consumer;


    } // namespace cms_ex;
} // namespace iegad;


#endif // __AMQ_CONSUMER__