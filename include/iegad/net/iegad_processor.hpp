#ifndef __IEGAD_NET_PROCESSOR__
#define __IEGAD_NET_PROCESSOR__


#include <event2/event.h>
#include <string>


namespace iegad {
namespace net {


class IProcessor {
public:
	virtual void onConnected(evutil_socket_t fd) = 0;
	virtual void onDisconnected(evutil_socket_t fd) = 0;
	virtual void onMessage(const std::string& data) = 0;

}; // class IProcessor;


} // namespace net
} // namespace iegad


#endif // __IEGAD_NET_PROCESSOR__