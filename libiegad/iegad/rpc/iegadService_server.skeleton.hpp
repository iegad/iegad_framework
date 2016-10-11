#ifndef __IEGAD_SERVICE_HANDLER__
#define __IEGAD_SERVICE_HANDLER__


#include "iegadService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "iegad_basic_handler.hpp"


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;


using boost::shared_ptr;

using namespace  ::iegad::rpc;


class iegadServiceHandler : virtual public iegadServiceIf {
public:
    iegadServiceHandler() {}


    void func(response& _return, const request& req) {
        _return = basicHandler::func(req);
    }


    void action(const request& req) {
        basicHandler::action(req);
    }
}; // class iegadServiceHandler;


#endif // __IEGAD_SERVICE_HANDLER__;

