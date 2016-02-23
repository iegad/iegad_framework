#include "Astroboy.svr.hpp"
#include <thrift\transport\TBufferTransports.h>


void 
iegad::thrift_ex::Astroboy_svr::_init()
{
    using ::apache::thrift::concurrency::ThreadManager;
    using ::apache::thrift::concurrency::PlatformThreadFactory;
    using ::apache::thrift::server::TThreadPoolServer;
    using ::apache::thrift::protocol::TBinaryProtocolFactory;
    using ::apache::thrift::transport::TBufferedTransportFactory;
    using ::apache::thrift::transport::TServerSocket;

    threadManager_ = ThreadManager::newSimpleThreadManager(threadCount_);
    threadManager_->threadFactory(
	boost::shared_ptr<PlatformThreadFactory>(new PlatformThreadFactory()));
    threadManager_->start();

    server_ = boost::shared_ptr<TThreadPoolServer>(new TThreadPoolServer(
	boost::make_shared<AstroboyProcessorFactory>(
	boost::make_shared<AstroboyCloneFactory>()),
	boost::make_shared<TServerSocket>(port_),
	boost::make_shared<TBufferedTransportFactory>(),
	boost::make_shared<TBinaryProtocolFactory>(),
	threadManager_));
}

