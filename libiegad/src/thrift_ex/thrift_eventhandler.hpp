#ifndef __THRIFT_EVENTHANDLER__
#define __THRIFT_EVENTHANDLER__



#include "thrift_ex_macro.h"
#include <thrift/server/TServer.h>
#include <functional>



// ============================
// @��; : CreateContextEvent�¼������
// @__func_name_ : ��������
// @inputProtocol : inputЭ���β�
// @outputProtocol : outputЭ���β�
// @����ֵ : void* Ϊ�Զ��������Ķ���ĵ�ַ
// @PS : �ͻ�������ʱ����
// ============================
#define DEFINE_CREATE_CONTEXT(__func_name_, inputProtocol, outputProtocol) \
void* __func_name_(boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol)


// ============================
// @��; : DeleteContextEvent�¼������
// @__func_name_ : ��������
// @serverContext : �����������Ļ���
// @inputProtocol : inputЭ���β�
// @outputProtocol : outputЭ���β�
// @PS : �ͻ��˹ر�ͨ������ʱ����
// ============================
#define DEFINE_DELETE_CONTEXT(__func_name_, serverContext, inputProtocol, outputProtocol) \
void __func_name_(void* serverContext, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol)


// ============================
// @��; : ProcessContextEvent�¼������
// @__func_name_ : ��������
// @serverContext : �����������Ļ���
// @trans : �ͻ���ͨ�Ŷ���
// @PS : �ͻ��˷���RPC��������ʱ����
// ============================
#define DEFINE_PROCESS_CONTEXT(__func_name_, serverContext, trans) \
void __func_name_(void* serverContext, boost::shared_ptr<::apache::thrift::transport::TTransport> trans)


// ============================
// @��; : PreServerEvent�¼������
// @PS : ���������ʱ����
// ============================
#define DEFINE_PRE_SERVER(__func_name_) \
void __func_name_()



// ============================
// @��; : thirft::TServerEventHandler�����ඨ���
// @className : ����������
// ============================
namespace iegad { 
namespace thrift_ex { 


    class ServerEventImpl : public ::apache::thrift::server::TServerEventHandler {
// ������¼���չ
    public:
	// ============================
	// @��; : �������Ͷ���
	// ============================
	typedef boost::shared_ptr<::apache::thrift::protocol::TProtocol> TProtocol_ptr;
	typedef boost::shared_ptr<::apache::thrift::transport::TTransport> TTransport_ptr;


	// ============================
	// @��; : �¼����Ͷ���
	// ============================
	DEFINE_EVENT(createContextEvent_t, void*(TProtocol_ptr, TProtocol_ptr))
	    DEFINE_EVENT(deleteContextEvent_t, void(void*, TProtocol_ptr, TProtocol_ptr))
	    DEFINE_EVENT(processContextEvent_t, void(void*, TTransport_ptr))
	    DEFINE_EVENT(preServeEvent_t, void(void))


	    // ============================
	    // @��; : ���캯��
	    // ============================
	    ServerEventImpl()
	    :
	    PreServeEvent(nullptr),
	    CreateContextEvent(nullptr),
	    DeleteContextEvent(nullptr),
	    ProcessContextEvent(nullptr)
	{}


	// ============================
	// @��; : ��������
	// ============================
	virtual ~ServerEventImpl() {}


	// ============================
	// @��; : �����ͻ��������Ļ���, �ڿͻ�������ʱ����.
	// @inputProtocol : ������Э��
	// @outputProtocol : �����Э��
	// @����ֵ : �����Ķ����ַ
	// @PS : ��TNonBlockingServer�� inputProtocol, outputProtocol
	//			��������������
	// ============================
	virtual void* createContext(boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol,
	    boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol) override {
	    if (CreateContextEvent != nullptr) {
		return CreateContextEvent(inputProtocol, outputProtocol);
	    }
	    return nullptr;
	}


	// ============================
	// @��; : ɾ���ͻ��������Ļ���, �ڿͻ��˶Ͽ�ʱ����.
	// @serverContext : �����Ļ�������.
	// @inputProtocol : ������Э��
	// @outputProtocol : �����Э��
	// @����ֵ : void
	// @PS : ��TNonBlockingServer�� inputProtocol, outputProtocol
	//			��������������
	// ============================
	virtual void deleteContext(void* serverContext,
	    boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol,
	    boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol) override {
	    if (DeleteContextEvent != nullptr) {
		DeleteContextEvent(serverContext, inputProtocol, outputProtocol);
	    }
	}


	// ============================
	// @��; : ����ͻ��������Ļ���, �ڿͻ��˵���RPCʱ, ����ʱ����.
	// @serverContext : �����Ļ�������.
	// @trans : ͨ�Ŷ���
	// @����ֵ : void
	// ============================
	virtual void processContext(void* serverContext,
	    boost::shared_ptr<::apache::thrift::transport::TTransport> trans) override {
	    if (ProcessContextEvent != nullptr) {
		ProcessContextEvent(serverContext, trans);
	    }
	}


	// ============================
	// @��; : �ڷ�������ǰ����, �ڷ��������ʱ����.
	// @����ֵ : void
	// ============================
	virtual void preServe() override {
	    if (PreServeEvent != nullptr) {
		PreServeEvent();
	    }
	}

	// ============================
	// @��; : �¼�ʵ������
	// ============================
	preServeEvent_t PreServeEvent;
	createContextEvent_t CreateContextEvent;
	deleteContextEvent_t DeleteContextEvent;
	processContextEvent_t ProcessContextEvent;


    private:
	// ============================
	// @��; : �� ��
	// ============================
	ServerEventImpl(const ServerEventImpl &);
	ServerEventImpl & operator=(const ServerEventImpl &);
    }; // class ServerEventImpl;
 

} // namespace thrift_ex;
} // namespace iegad;


// ============================
// @��; : ������¼������� �����(�ӿں�, ���û�ʹ�õĲ���)
// @__class_name_ : ����
// ============================
#define DEFINE_EVENT_HANDLER(__class_name_) \
typedef iegad::thrift_ex::ServerEventImpl __class_name_;


#endif // __THRIFT_EVENTHANDLER__