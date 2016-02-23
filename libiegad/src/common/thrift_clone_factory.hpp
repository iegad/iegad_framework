#ifndef __THRIFT_CLONE_FACTORY__
#define __THRIFT_CLONE_FACTORY__



// ============ ˵�� ============
//
// @�������� : 2016-02-23
// @������ : iegad
//
// ============================
// @��; : 1, ��thrift ThreadPoolServer����չ ����ʹ��.
//
//		AstroboyCloneFactory : AstroboyHandler������, 
//		�ṩ ThreadPoolServer ʹ��
//		
// @PS : ���ļ������� thrift ��Դ��    
//
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================



#include "gen-cpp/Astroboy_server.skeleton.hpp"



namespace iegad {
namespace thrift_ex {


    class AstroboyCloneFactory : virtual public AstroboyIfFactory {
    // AstroboyHandler����
    public:
	// ============================
	// @��; : ��������
	// ============================
	virtual ~AstroboyCloneFactory() {}


	// ============================
	// @��; : ���� XXX_Handler ����ʵ��
	// @connInfo : �ͻ���������Ϣ.
	// @����ֵ : XXX_Handler ʵ��.
	// ============================
	virtual AstroboyIf * getHandler(const ::apache::thrift::TConnectionInfo & connInfo) {
	    // ����������鿴�����ӵ� �ͻ��� ����Ϣ
	    /*boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
	    std::cout << "Client connected : " << sock->getOrigin() << '\n' << std::endl;*/
	    return new AstroboyHandler;
	}


	// ============================
	// @��; : �ͷ� handler ����.
	// @handler : ��Ҫ�ͷŵ���Դ����.
	// @����ֵ : void
	// ============================
	virtual void releaseHandler(AstroboyIf * handler) {
	    delete handler;
	}
    }; // class AstroboyCloneFactory;


} // namespace thrift_ex;
} // namespace iegad;




#endif // __THRIFT_CLONE_FACTORY__