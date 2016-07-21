#ifndef __IEGAD_TCP_EVENT__
#define __IEGAD_TCP_EVENT__


#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>


namespace iegad {
namespace net {


class tcp_session;


struct tcp_event {
private:
    typedef boost::shared_ptr<tcp_session> tcp_session_ptr;


public:
    typedef boost::shared_ptr<tcp_event> ptr_t;

    boost::function<void(tcp_session_ptr)> open_handler;
    boost::function<void(tcp_session_ptr)> close_handler;

    boost::function<void(tcp_session_ptr, size_t)> read_handler;
    boost::function<void(tcp_session_ptr, size_t)> write_handler;
}; // class tcp_event;


} // namespace net;
} // namespace iegad;



#endif // __IEGAD_TCP_EVENT__
