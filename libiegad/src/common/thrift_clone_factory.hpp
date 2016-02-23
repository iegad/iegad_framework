#ifndef __THRIFT_CLONE_FACTORY__
#define __THRIFT_CLONE_FACTORY__



// ============ 说明 ============
//
// @创建日期 : 2016-02-23
// @创建人 : iegad
//
// ============================
// @用途 : 1, 对thrift ThreadPoolServer的扩展 方便使用.
//
//		AstroboyCloneFactory : AstroboyHandler工厂类, 
//		提供 ThreadPoolServer 使用
//		
// @PS : 该文件依赖于 thrift 开源库    
//
// ============================
//
// @修改记录:
// =======================================
//  日期                     修改人                                   修改说明
// =======================================



#include "gen-cpp/Astroboy_server.skeleton.hpp"



namespace iegad {
namespace thrift_ex {


    class AstroboyCloneFactory : virtual public AstroboyIfFactory {
    // AstroboyHandler工厂
    public:
	// ============================
	// @用途 : 析构函数
	// ============================
	virtual ~AstroboyCloneFactory() {}


	// ============================
	// @用途 : 构建 XXX_Handler 对象实例
	// @connInfo : 客户端连接信息.
	// @返回值 : XXX_Handler 实例.
	// ============================
	virtual AstroboyIf * getHandler(const ::apache::thrift::TConnectionInfo & connInfo) {
	    // 可以在这里查看所连接的 客户端 的信息
	    /*boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
	    std::cout << "Client connected : " << sock->getOrigin() << '\n' << std::endl;*/
	    return new AstroboyHandler;
	}


	// ============================
	// @用途 : 释放 handler 对象.
	// @handler : 需要释放的资源对象.
	// @返回值 : void
	// ============================
	virtual void releaseHandler(AstroboyIf * handler) {
	    delete handler;
	}
    }; // class AstroboyCloneFactory;


} // namespace thrift_ex;
} // namespace iegad;




#endif // __THRIFT_CLONE_FACTORY__