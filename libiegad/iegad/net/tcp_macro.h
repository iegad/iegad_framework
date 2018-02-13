#ifndef __TCP_MACRO__
#define __TCP_MACRO__



#include <event.h>
#include <assert.h>
#include <unistd.h>
#include <boost/shared_ptr.hpp>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <thread>



namespace iegad {
namespace net {



enum TcpErrorEvent {
    onAccept,        // 接收客户端连接错误, arg: errno
    onProcTimeout,   // 客户端处理超时, arg: sockfd
    onRead,
    onWrite,
    onEOF
};



} // namespace net;
} // namespace iegad;



#endif // __TCP_MACRO__
