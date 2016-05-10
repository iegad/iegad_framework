#ifndef __THRIFT_EVENTHANDLER__
#define __THRIFT_EVENTHANDLER__



#include "thrift_ex_macro.h"
#include <thrift/server/TServer.h>
#include <functional>



// ============================
// @��; : CreateContextEvent�¼������
// @funcName : ��������
// @inputProtocol : inputЭ���β�
// @outputProtocol : outputЭ���β�
// @����ֵ : void* Ϊ�Զ��������Ķ���ĵ�ַ
// @PS : �ͻ�������ʱ����
// ============================
#define DEFINE_CREATE_CONTEXT(funcName, inputProtocol, outputProtocol) \
void* funcName(boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol)


// ============================
// @��; : DeleteContextEvent�¼������
// @funcName : ��������
// @serverContext : �����������Ļ���
// @inputProtocol : inputЭ���β�
// @outputProtocol : outputЭ���β�
// @PS : �ͻ��˹ر�ͨ������ʱ����
// ============================
#define DEFINE_DELETE_CONTEXT(funcName, serverContext, inputProtocol, outputProtocol) \
void funcName(void* serverContext, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol)


// ============================
// @��; : ProcessContextEvent�¼������
// @funcName : ��������
// @serverContext : �����������Ļ���
// @trans : �ͻ���ͨ�Ŷ���
// @PS : �ͻ��˷���RPC��������ʱ����
// ============================
#define DEFINE_PROCESS_CONTEXT(funcName, serverContext, trans) \
void funcName(void* serverContext, boost::shared_ptr<::apache::thrift::transport::TTransport> trans)


// ============================
// @��; : PreServerEvent�¼������
// @PS : ���������ʱ����
// ============================
#define DEFINE_PRE_SERVER(funcName) \
void funcName()



// ============================
// @��; : thirft::TServerEventHandler�����ඨ���
// @className : ����������
// ============================
#define THRIFT_EVENT_HANDLER(className) \
namespace iegad { \
namespace thrift_ex { \
 \
class className : public ::apache::thrift::server::TServerEventHandler { \
public: \
\
	typedef std::function<void*(boost::shared_ptr<::apache::thrift::protocol::TProtocol>, \
	boost::shared_ptr<::apache::thrift::protocol::TProtocol>)> createContextEvent_t; \
\
	typedef std::function<void(void*, \
	boost::shared_ptr<::apache::thrift::protocol::TProtocol>, \
	boost::shared_ptr<::apache::thrift::protocol::TProtocol>)> deleteContextEvent_t; \
\
	typedef std::function<void(void*, \
	boost::shared_ptr<::apache::thrift::transport::TTransport>)> processContextEvent_t; \
\
	typedef std::function<void(void)> preServeEvent_t; \
\
	className () \
	: \
	PreServeEvent(nullptr), \
	CreateContextEvent(nullptr), \
	DeleteContextEvent(nullptr), \
	ProcessContextEvent(nullptr) \
	{} \
\
	virtual ~className() {} \
\
	virtual void* createContext(boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
										boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol) override { \
		if (CreateContextEvent != nullptr) { \
			return CreateContextEvent(inputProtocol, outputProtocol); \
		} \
		return nullptr; \
	} \
	 \
	virtual void deleteContext(void* serverContext, \
										boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
										boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol) override { \
		if (DeleteContextEvent != nullptr) { \
			DeleteContextEvent(serverContext, inputProtocol, outputProtocol); \
		} \
	} \
	 \
	virtual void processContext(void* serverContext, \
										boost::shared_ptr<::apache::thrift::transport::TTransport> trans) override { \
		if (ProcessContextEvent != nullptr) { \
			ProcessContextEvent(serverContext, trans); \
		} \
	} \
\
	virtual void preServe() override { \
		if (PreServeEvent != nullptr) { \
			PreServeEvent(); \
		} \
	} \
 \
public: \
	preServeEvent_t PreServeEvent; \
	createContextEvent_t CreateContextEvent; \
	deleteContextEvent_t DeleteContextEvent; \
	processContextEvent_t ProcessContextEvent; \
}; \
 \
} \
}


// ============================
// @��; : ������¼������� �����(�ӿں�, ���û�ʹ�õĲ���)
// @className : ����
// ============================
#define DEFINE_EVENT_HANDLER(className) THRIFT_EVENT_HANDLER(className) 


#endif // __THRIFT_EVENTHANDLER__