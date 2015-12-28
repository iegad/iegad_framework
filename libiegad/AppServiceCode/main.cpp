#include <iostream>
#include <glog/logging.h>
#include "AppServiceImp.h"
#include "Environment.h"



int
main(int argc, char * argv[])
{
    LOG_INIT(argv[0]);    
    InitEnvironment();
    //if (argc < 3) {
    //    LOG(INFO) <<"Process Augment Error\n";
    //    exit(1);
    //}

    int port = 2015;
    const int workerCount = 4;
    std::string redisConnString = "192.168.1.210:6379";

     boost::shared_ptr<ThreadManager> threadManager =
        ThreadManager::newSimpleThreadManager(workerCount);
     threadManager->threadFactory(
        boost::make_shared<PlatformThreadFactory>());
     threadManager->start();


    shared_ptr<AppServiceHandler> handler(new AppServiceHandler(redisConnString));
    shared_ptr<TProcessor> processor(new AppServiceProcessor(handler));
    shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());


    TThreadPoolServer server
                            (processor,
                             serverTransport,
                             transportFactory,
                             protocolFactory,
                             threadManager);
    LOG(INFO) << "Service Runing...\n";
    server.serve();
    LOG_RELEASE();
    exit(0);
}

