#ifndef __ASTROBOY_SVR__
#define __ASTROBOY_SVR__



// ============ ˵�� ============
//
// @�������� : 2016-02-23
// @������ : iegad
//
// ============================
// @��; : 1, ��thrift ThreadPoolServer����չ

// @ Astroboy_svr : ����˽���,
//			    ʹ�� ThreadPoolServer ��Ϊʵ��.
//			    ����Э��ʹ�� BinaryProtocol .
//		
// @PS : ���ļ������� thrift ��Դ��    
//
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================


#define VERSION_TYPE "V0.1.0"	    // ��ǰ thrift_ex �汾��.



#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TSocket.h>

#include "thrift_clone_factory.hpp"

#include <boost/make_shared.hpp>

#include <iostream>
#include <functional>


namespace iegad {
namespace thrift_ex {


    class Astroboy_svr {
    // Astroboy ���̳߳� Server��
    public:
	typedef std::function<bool(void)> fun_t;
	typedef AstroboyHandler::action_map_t action_map_t;

	// ============================
	// @��; : ��ʼ������.
	// @func : �ɵ������ṩ������ʼ���ص�����
	// @����ֵ : ��ʼ���ɹ�, ���� true, ���򷵻� false.
	// ============================
	static bool InitEnvironment(fun_t func) {
	    return func == nullptr ? false : func();
	}


	// ============================
	// @��; : ���캯��.
	// @port : �˿ں�.
	// @threadCount : �����߳���
	// ============================
	explicit Astroboy_svr(int port, int threadCount = 4)
	    :
	    threadCount_(threadCount),
	    port_(port),
	    server_(nullptr) {
	    this->_init();
	}


	// ============================
	// @��; : ���������� Server.
	// @����ֵ : void
	// ============================
	void Run() {
	    std::cout << "===============================\n";
	    std::cout << VERSION_TYPE << std::endl;
	    std::cout << "Server running...\nlistened at 0.0.0.0 : " << port_ << std::endl;
	    std::cout << "===============================\n\n";
	    server_->run();
	}


    private:
	// ============================
	// @��; : ���ó�ʼ��
	// @����ֵ : void
	// ============================
	void _init();


	// �˿�
	int port_;
	// �����߳���
	int threadCount_;
	// �̵߳�����
	boost::shared_ptr<::apache::thrift::concurrency::ThreadManager> threadManager_;
	// TThreadPoolServer ����ʵ����ָ��
	boost::shared_ptr<::apache::thrift::server::TThreadPoolServer> server_;


	// ============================
	// @��; : ��ֹ ���� ��ֵ����
	// ============================
	Astroboy_svr(const Astroboy_svr &);
	Astroboy_svr &  operator=(const Astroboy_svr &);
    }; // class ServerHost;


} // namespace thrift_ex;
} // namespace iegad;



#endif // __ASTROBOY_SVR__