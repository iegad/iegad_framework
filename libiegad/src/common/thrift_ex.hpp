#ifndef __THRIFT_EX__
#define __THRIFT_EX__


// ============ ˵�� ============
//
// @�������� : 2016-02-20
// @������ : iegad
//
// ============================
// @��; : 1, ��thrift ThreadPoolServer����չ
//		����ʹ��.
//		2, ��thirft �����ɵĽṹ�ṩ���л�����.
//		
//		@PS : ���ļ������� thrift ��Դ��    
//		
//		@ Serializer : ���л���, 
//			    �������л��뷴���л�, �ṩ ������ �� JSON�������л���ʽ.
//
//		@ BasicCloneFactory : Handler����, 
//			    ����XXX_Handler ����ʵ���� ����������.
//			    ����̳����� thrift ���ɵ� XXX_If �ӿ�.
//
//		@ ServerHost : ����˽���, 
//			    ʹ�� ThreadPoolServer ��Ϊʵ��.
//			    ����Э��ʹ�� BinaryProtocol .
// ============================
//
// @�޸ļ�¼:
// =======================================
//  ����                     �޸���                                   �޸�˵��
// =======================================



// ============ ���� ============
//  �ڰ�����ͷ�ļ�ʱ, �������� 
//	XXX_IfFactory, XXX_Handler, XXX_If, XXX_ProcessorFactory 
//  ������Щ��.
//  ��ֵ ��thrift �����ɵĶ�Ӧ����, �滻 XXX_ ����.
// ===========================
#if defined(XXX_IfFactory) && defined(XXX_Handler) && defined(XXX_If) && defined(XXX_ProcessorFactory)


#define VERSION_TYPE "V0.0.1"	    // ��ǰ thrift_ex �汾��.


#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/server/TThreadPoolServer.h>


#include <boost/make_shared.hpp>
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>


#include <iostream>


using namespace apache::thrift::concurrency;


template <class T>
class Serializer {
// thrift ���л���.
public:
    // ============================
    // @��; : ��ֹ ʵ��������
    // ============================
    virtual ~Serializer() = 0;

    // ============================
    // @��; : �� T ���л�Ϊ �ֽ� ����
    // @data : ��thrift ���ɵ� struct ����
    // @serstr : ���л�������� : out����
    // @size : ���л�������ݴ�С : out����
    // @����ֵ : �ɹ����� true, ���򷵻� false.
    // ============================
    static bool SerializeToArray(T & data, std::string * serstr, int * size) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TBinaryProtocol> proto(new TBinaryProtocol(buff));
	    data.write(proto.get());
	    uint8_t * p;
	    buff->getBuffer(&p, (uint32_t *)size);
	    *serstr = std::string((char *)p, *size);
	    return true;
	}
	catch (std::exception &) {
	    //...
	}
	return false;
    }


    // ============================
    // @��; : �� �ֽ����� �����л�Ϊ T ���ݶ���
    // @data : ��thrift ���ɵ� struct ����ָ�� : out����
    // @serstr : ���ڷ����л��� �ֽ�����
    // @����ֵ : �ɹ����� true, ���򷵻� false.
    // ============================
    static bool ParserFromArray(T * data, const std::string & serstr) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TProtocol> proto(new TBinaryProtocol(buff));
	    uint8_t * p = (uint8_t *)const_cast<char *>(serstr.c_str());
	    buff->resetBuffer(p, serstr.size());
	    data->read(proto.get());
	    return true;
	}
	catch (std::exception &) {
	
	}
	return false;
    }


    // ============================
    // @��; : �� T ���л�Ϊ JSON ��
    // @data : ��thrift ���ɵ� struct ����
    // @serstr : ���л���� JSON�� : out����
    // @size : ���л���� JSON������ : out����
    // @����ֵ : �ɹ����� true, ���򷵻� false.
    // ============================
    static bool SerializeToJSONString(T & data, std::string * serstr, int * size) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TJSONProtocol> proto(new TJSONProtocol(buff));
	    data.write(proto.get());
	    uint8_t * p;
	    buff->getBuffer(&p, (uint32_t *)size);
	    *serstr = std::string((char *)p, *size);
	    return true;
	}
	catch (std::exception &) {
	    //...
	}
	return false;
    }


    // ============================
    // @��; : �� JSON�� �����л�Ϊ T ���ݶ���
    // @data : ��thrift ���ɵ� struct ����ָ�� : out����
    // @serstr : ���ڷ����л��� JSON��
    // @����ֵ : �ɹ����� true, ���򷵻� false.
    // ============================
    static bool ParserFromJSONString(T * data, const std::string & serstr) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TProtocol> proto(new TJSONProtocol(buff));
	    uint8_t * p = (uint8_t *)const_cast<char *>(serstr.c_str());
	    buff->resetBuffer(p, serstr.size());
	    data->read(proto.get());
	    return true;
	}
	catch (std::exception &) {

	}
	return false;
    }
}; // class Serializer;


class BasicCloneFactory : virtual public XXX_IfFactory {
// Handler����
public:
    // ============================
    // @��; : ��������
    // ============================
    virtual ~BasicCloneFactory() {}


    // ============================
    // @��; : ���� XXX_Handler ����ʵ��
    // @connInfo : �ͻ���������Ϣ.
    // @����ֵ : XXX_Handler ʵ��.
    // ============================
    virtual XXX_If* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
	// ����������鿴�����ӵ� �ͻ��� ����Ϣ
	boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
	std::cout << "Client connected : " << sock->getOrigin() << '\n' << std::endl;
	return new XXX_Handler;
    }


    // ============================
    // @��; : �ͷ� handler ����.
    // @handler : ��Ҫ�ͷŵ���Դ����.
    // @����ֵ : void
    // ============================
    virtual void releaseHandler(XXX_If * handler) {
	delete handler;
    }
}; // class BasicCloneFactory;


class ServerHost {
// ���̳߳� Server��
public:
    // ============================
    // @��; : ��ʼ������.
    // @����ֵ : ��ʼ���ɹ�, ���� true, ���򷵻� false.
    // @PS : XXX_Handler �����ṩInitEvironment ����, 
    //		   �ú���������thrift ����, �����ֶ��޸�.
    //		   �ú��������ڹ���ServerHost ʵ��֮ǰ������.
    // ============================
    static bool InitEvironment() {
	return XXX_Handler::InitEvironment();
    }


    // ============================
    // @��; : ���캯��.
    // @port : �˿ں�.
    // @threadCount : �����߳���
    // ============================
    explicit ServerHost(int port, int threadCount = 4)
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
    void _init() {

	threadManager_ = ThreadManager::newSimpleThreadManager(threadCount_);
	threadManager_->threadFactory(
	    shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory()));
	threadManager_->start();

	server_ = shared_ptr<TThreadPoolServer>(new TThreadPoolServer(
	    boost::make_shared<XXX_ProcessorFactory>(
	    boost::make_shared<BasicCloneFactory>()),
	    boost::make_shared<TServerSocket>(port_),
	    boost::make_shared<TBufferedTransportFactory>(),
	    boost::make_shared<TBinaryProtocolFactory>(),
	    threadManager_));
    }


    // �˿�
    int port_;
    // �����߳���
    int threadCount_;
    // �̵߳�����
    shared_ptr<ThreadManager> threadManager_;
    // TThreadPoolServer ����ʵ����ָ��
    shared_ptr<TThreadPoolServer> server_;


    // ============================
    // @��; : ��ֹ ���� ��ֵ����
    // ============================
    ServerHost(const ServerHost &);
    ServerHost &  operator=(const ServerHost &);
}; // class ServerHost;


#endif // #if defined(XXX_IfFactory) && defined(XXX_Handler) && ...
#endif // !__THRIFT_EX__
