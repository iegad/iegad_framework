#ifndef __AMQ_BASER__
#define __AMQ_BASER__



// ============ ˵�� ============
//
// @�������� : 2016-02-21
// @������ : iegad
//
// ============================
// @��; : 1, ��ActiveMQ-CMS ����չ, 
//		2, AMQ_Baser ������ AMQ��չ�� �Ļ���
//		
// @PS : ���ļ������� ActiveMQ-CMS ��Դ��
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================



#include <activemq/library/ActiveMQCPP.h>
#include <activemq/core/ActiveMQConnectionFactory.h>



namespace iegad {
namespace cms_ex {


    inline void InitActiveMQEnvironment() {
	activemq::library::ActiveMQCPP::initializeLibrary();
    }


    class AMQ_Baser {
    //AMQ ���� (������)
    public:
        // ============================
        // @��; : ���캯��
        // @PS : �ṩĬ�Ϲ����Ŀ���ǿ��Խ�� ����ģ�� ��ʹ��.
        // ============================
	AMQ_Baser() {}
    
    
        // ============================
        // @��; : ��������, ��ֹ ���౻ʵ����
        // ============================
	virtual ~AMQ_Baser() = 0 { 
	    this->_cleanup(); 
	}
    
    
        // ============================
        // @��; : ����AMQ���������
        // @errstr : ������Ϣ : out����
        // @����ֵ : �ɹ����� true, ���򷵻� false.
        // ============================
        bool Open(std::string * errstr = nullptr);   
    
    
        // ============================
        // @��; : ���û���
        // @uri : AMQ��URI.	    �� : "tcp://localhost:61616"
        // @persistent_flag : ִ�л���־, 0��ʾִ�û�, 1 ��ʾ��ִ�û�
        // @amq_name : AMQ��������
        // @usr : ������Ϣ : �û���
        // @pwd : ������Ϣ : ����
        // @connTimeout : ���ӳ�ʱ  ��λ : ����
        // @sendTimeout : ���ͳ�ʱ  ��λ : ����
        // @����ֵ : void
        // ============================
        void SetEnvironment(const std::string & uri,
	    int persistent_flag,
	    const std::string & amq_name,
	    const std::string & usr = "",
	    const std::string & pwd = "",
	    int32_t connTimeout = -1,
	    int32_t sendTimeout = -1);
    
    
        // ============================
        // @��; : ��ȡBrokerUri
        // @����ֵ : ��ǰBrokerUri
        // ============================
        const std::string & GetBrokerURI() const {
    	    return uri_;
        }
    
    
        // ============================
        // @��; : ��ȡ �����֤ : �û���
        // @����ֵ : ��ǰ�û���
        // ============================
        const std::string & GetUser() const {
    	    return usr_;
        }
    
    
        // ============================
        // @��; : ��ȡ �����֤ : ����
        // @����ֵ : ��ǰ����
        // ============================
        const std::string & GetPassword() const {
    	    return pwd_;
        }
    
    
        // ============================
        // @��; : ��ȡ ���ӳ�ʱ
        // @����ֵ : ���ӳ�ʱ
        // ============================
        int32_t GetConnectTimeout() const {
    	    return connTimeout_;
        }
    
    
        // ============================
        // @��; : ��ȡ ���ͳ�ʱ
        // @����ֵ : ���ͳ�ʱ
        // ============================
        int32_t GetSendTimeout() const {
    	    return sendTimeout_;
        }
    
    protected:
        // ============================
        // @��; : ����BrokerUri��
        // @����ֵ : ����ɹ���BrokerUri�ַ���
        // ============================
        const std::string _make_broker_uri();
    
    
        // ============================
        // @��; : ��ʼ���ڲ��ֶ�ֵ
        // @����ֵ : void
        // ============================
        virtual void _init();
    
    
        // ============================
        // @��; : �ͷ���ռ�õ�ϵͳ��Դ
        // @����ֵ : void
        // ============================
        virtual void _cleanup();
    
    
        // ���л���־
        int persistent_flag_;
        // ���ӳ�ʱ
        int32_t connTimeout_;
        // ���ͳ�ʱ
        int32_t sendTimeout_;
    
        // ���Ӷ���ָ��
        std::shared_ptr<::cms::Connection> conn_;
        // �Ựָ��
        std::shared_ptr<::cms::Session> session_;
        // Ŀ��ָ��
        std::shared_ptr<::cms::Destination> dest_;
        // AMQ���Ӷ��󹤳�
        std::shared_ptr<::activemq::core::ActiveMQConnectionFactory> connFactory_;
    
        // �û���
        std::string usr_;
        // ����
        std::string pwd_;
        // BrokerUri
        std::string uri_;
        // AMQ��������
        std::string amq_name_;

    private:
	// ============================
	// @��; : ��ֹ��ֵ����
	// ============================
	AMQ_Baser(AMQ_Baser &);
	AMQ_Baser& operator=(AMQ_Baser &);
    }; // class AMQ_Baser;


} // namespace cms_ex;
} // namespace iegad;



#endif __AMQ_BASER__