#ifndef __THRIFT_EVENTHANDLER__
#define __THRIFT_EVENTHANDLER__



#include "thrift_ex_macro.h"
#include <thrift/server/TServer.h>
#include <functional>



// ============================
// @用途 : CreateContextEvent事件句柄宏
// @__func_name_ : 函数名称
// @inputProtocol : input协议形参
// @outputProtocol : output协议形参
// @返回值 : void* 为自定义上下文对象的地址
// @PS : 客户端连接时触发
// ============================
#define DEFINE_CREATE_CONTEXT(__func_name_, inputProtocol, outputProtocol) \
void* __func_name_(boost::shared_ptr<apache::thrift::protocol::TProtocol> inputProtocol, \
boost::shared_ptr<apache::thrift::protocol::TProtocol> outputProtocol)


// ============================
// @用途 : DeleteContextEvent事件句柄宏
// @__func_name_ : 函数名称
// @serverContext : 给定的上下文环境
// @inputProtocol : input协议形参
// @outputProtocol : output协议形参
// @PS : 客户端关闭通信连接时触发
// ============================
#define DEFINE_DELETE_CONTEXT(__func_name_, serverContext, inputProtocol, outputProtocol) \
void __func_name_(void* serverContext, \
boost::shared_ptr<apache::thrift::protocol::TProtocol> inputProtocol, \
boost::shared_ptr<apache::thrift::protocol::TProtocol> outputProtocol)


// ============================
// @用途 : ProcessContextEvent事件句柄宏
// @__func_name_ : 函数名称
// @serverContext : 给定的上下文环境
// @trans : 客户端通信对象
// @PS : 客户端发出RPC调用请求时触发
// ============================
#define DEFINE_PROCESS_CONTEXT(__func_name_, serverContext, trans) \
void __func_name_(void* serverContext, boost::shared_ptr<apache::thrift::transport::TTransport> trans)


// ============================
// @用途 : PreServerEvent事件句柄宏
// @PS : 服务端启动时触发
// ============================
#define DEFINE_PRE_SERVER(__func_name_) \
void __func_name_()



// ============================
// @用途 : thirft::TServerEventHandler派生类定义宏
// @className : 派生类类名
// ============================
namespace iegad { 
namespace thrift_ex { 


class ServerEventImpl : public ::apache::thrift::server::TServerEventHandler {
// 服务端事件扩展
public:
	// ============================
	// @用途 : 内置类型定义
	// ============================
    typedef boost::shared_ptr<apache::thrift::protocol::TProtocol> TProtocol_ptr;
    typedef boost::shared_ptr<apache::thrift::transport::TTransport> TTransport_ptr;


	// ============================
	// @用途 : 事件类型定义
	// ============================
    typedef std::function<void*(TProtocol_ptr, TProtocol_ptr)> createContextEvent_t;
    typedef std::function<void(void*, TProtocol_ptr, TProtocol_ptr)> deleteContextEvent_t;
    typedef std::function<void(void*, TTransport_ptr)> processContextEvent_t;
    typedef std::function<void(void)> preServeEvent_t;


    // ============================
    // @用途 : 构造函数
    // ============================
    ServerEventImpl()
	    :
	    PreServeEvent(nullptr),
	    CreateContextEvent(nullptr),
	    DeleteContextEvent(nullptr),
	    ProcessContextEvent(nullptr)
	{}


	// ============================
	// @用途 : 析构函数
	// ============================
	virtual ~ServerEventImpl() {}


	// ============================
	// @用途 : 创建客户端上下文环境, 在客户端连接时触发.
	// @inputProtocol : 输入流协议
	// @outputProtocol : 输出流协议
	// @返回值 : 上下文对象地址
	// @PS : 在TNonBlockingServer下 inputProtocol, outputProtocol
	//			两个参数无意义
	// ============================
    virtual void* createContext(boost::shared_ptr<apache::thrift::protocol::TProtocol> inputProtocol,
        boost::shared_ptr<apache::thrift::protocol::TProtocol> outputProtocol) override {
	    if (CreateContextEvent != nullptr) {
            return CreateContextEvent(inputProtocol, outputProtocol);
	    }
	    return nullptr;
	}


	// ============================
	// @用途 : 删除客户端上下文环境, 在客户端断开时触发.
	// @serverContext : 上下文环境对象.
	// @inputProtocol : 输入流协议
	// @outputProtocol : 输出流协议
	// @返回值 : void
	// @PS : 在TNonBlockingServer下 inputProtocol, outputProtocol
	//			两个参数无意义
	// ============================
	virtual void deleteContext(void* serverContext,
        boost::shared_ptr<apache::thrift::protocol::TProtocol> inputProtocol,
        boost::shared_ptr<apache::thrift::protocol::TProtocol> outputProtocol) override {
	    if (DeleteContextEvent != nullptr) {
		DeleteContextEvent(serverContext, inputProtocol, outputProtocol);
	    }
	}


	// ============================
	// @用途 : 处理客户端上下文环境, 在客户端调用RPC时, 连接时触发.
	// @serverContext : 上下文环境对象.
	// @trans : 通信对象
	// @返回值 : void
	// ============================
	virtual void processContext(void* serverContext,
        boost::shared_ptr<apache::thrift::transport::TTransport> trans) override {
	    if (ProcessContextEvent != nullptr) {
		ProcessContextEvent(serverContext, trans);
	    }
	}


	// ============================
	// @用途 : 在服务启动前运行, 在服务端启动时触发.
	// @返回值 : void
	// ============================
	virtual void preServe() override {
	    if (PreServeEvent != nullptr) {
		PreServeEvent();
	    }
	}

	// ============================
	// @用途 : 事件实例定义
	// ============================
	preServeEvent_t PreServeEvent;
	createContextEvent_t CreateContextEvent;
	deleteContextEvent_t DeleteContextEvent;
	processContextEvent_t ProcessContextEvent;


    private:
	// ============================
	// @用途 : 禁 用
	// ============================
	ServerEventImpl(const ServerEventImpl &);
	ServerEventImpl & operator=(const ServerEventImpl &);
}; // class ServerEventImpl;
 

} // namespace thrift_ex;
} // namespace iegad;


// ============================
// @用途 : 服务端事件处理类 定义宏(接口宏, 供用户使用的部分)
// @__class_name_ : 类名
// ============================
#define DEFINE_EVENT_HANDLER(__class_name_) \
typedef iegad::thrift_ex::ServerEventImpl __class_name_;


#endif // __THRIFT_EVENTHANDLER__
