#ifndef __THRIFT_EVENTHANDLER__
#define __THRIFT_EVENTHANDLER__


#include "thrift_ex_macro.h"
#include <thrift/server/TServer.h>
#include <functional>



#define DECLARE_CREATE_CONTEXT(funcName, inputProtocol, outputProtocol) \
void* funcName(boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol)


#define DECLARE_DELETE_CONTEXT(funcName, serverContext, inputProtocol, outputProtocol) \
void funcName(void* serverContext, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol)


#define DECLARE_PROCESS_CONTEXT(funcName, serverContext, trans) \
void funcName(void* serverContext, boost::shared_ptr<::apache::thrift::transport::TTransport> trans)


#define DECLARE_PRE_SERVER(funcName) \
void funcName()


#define THRIFT_EVENT_HANDLER(className) \
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
	OnPreServeEvent(nullptr), \
	OnCreateContextEvent(nullptr), \
	OnDeleteContextEvent(nullptr), \
	OnProcessContextEvent(nullptr) \
	{} \
	\
	virtual ~className() {} \
	\
	virtual void* createContext(boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
	boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol) override { \
		if (OnCreateContextEvent != nullptr) { \
			return OnCreateContextEvent(inputProtocol, outputProtocol); \
		} \
		return nullptr; \
	} \
	\
	virtual void deleteContext(void* serverContext, \
		boost::shared_ptr<::apache::thrift::protocol::TProtocol> inputProtocol, \
		boost::shared_ptr<::apache::thrift::protocol::TProtocol> outputProtocol) override { \
		if (OnDeleteContextEvent != nullptr) { \
			OnDeleteContextEvent(serverContext, inputProtocol, outputProtocol); \
		} \
	} \
	\
	virtual void processContext(void* serverContext, \
	boost::shared_ptr<::apache::thrift::transport::TTransport> trans) override { \
		if (OnProcessContextEvent != nullptr) { \
			OnProcessContextEvent(serverContext, trans); \
		} \
	} \
	\
	virtual void preServe() override { \
		if (OnPreServeEvent != nullptr) { \
			OnPreServeEvent(); \
		} \
	} \
	\
public: \
	preServeEvent_t OnPreServeEvent; \
	createContextEvent_t OnCreateContextEvent; \
	deleteContextEvent_t OnDeleteContextEvent; \
	processContextEvent_t OnProcessContextEvent; \
}; 


#define DECLARE_EVENT_HANDLER(className) THRIFT_EVENT_HANDLER(className) 


#endif // __THRIFT_EVENTHANDLER__