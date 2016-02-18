#ifndef __THRIFT_EX__
#define __THRIFT_EX__


#ifdef XXX_IfFactory && XXX_Handler && XXX_If && XXX_ProcessorFactory


#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/server/TThreadPoolServer.h>


#include <boost/make_shared.hpp>
#include <thrift/transport/TSocket.h>
#include <thrift/protocol/TJSONProtocol.h>


using namespace apache::thrift::concurrency;


template <class T>
class Serializer {
public:
    static bool SerializeToArray(T & data, std::string * serstr, int * size) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TBinaryProtocol> proto(new TBinaryProtocol(buff));
	    data.write(proto.get());
	    uint8_t * p;
	    buff->getBuffer(&p, size);
	    *serstr = std::string((char *)p, *size);
	    return true;
	}
	catch (std::exception &) {
	    //...
	}
	return false;
    }

    static bool ParserFromArray(T & data, const std::string & serstr) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TProtocol> proto(new TBinaryProtocol(buff));
	    uint8_t * p = (uint8_t *)const_cast<char *>(serstr.c_str());
	    buff->resetBuffer(p, serstr.size());
	    data.read(proto.get());
	    return true;
	}
	catch (std::exception &) {
	
	}
	return false;
    }

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

    static bool ParserFromJSONString(T & data, const std::string & serstr) {
	try {
	    boost::shared_ptr<TMemoryBuffer> buff(new TMemoryBuffer);
	    boost::shared_ptr<TProtocol> proto(new TJSONProtocol(buff));
	    uint8_t * p = (uint8_t *)const_cast<char *>(serstr.c_str());
	    buff->resetBuffer(p, serstr.size());
	    data.read(proto.get());
	    return true;
	}
	catch (std::exception &) {

	}
	return false;
    }
}; // class Serializer;


class BasicCloneFactory : virtual public XXX_IfFactory {
public:
    virtual ~BasicCloneFactory() {}
    virtual CenterBizIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
	boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
	return new XXX_Handler;
    }
    virtual void releaseHandler(XXX_If * handler) {
	delete handler;
    }
}; // class BasicCloneFactory;


class ServerHost {
public:
    explicit ServerHost(int port, int threadCount = 4)
	:
	threadCount_(threadCount),
	port_(port),
	server_(nullptr) {
	this->_init();
    }

    void Run() {
	server_->run();
    }

private:
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


    int port_;
    int threadCount_;
    shared_ptr<ThreadManager> threadManager_;
    shared_ptr<TThreadPoolServer> server_;


    ServerHost(const ServerHost &);
    ServerHost &  operator=(const ServerHost &);
}; // class ServerHost;


#endif // #ifdef XXX_IfFactory && XXX_Handler && XXX_If && XXX_ProcessorFactory
#endif // !__THRIFT_EX__
