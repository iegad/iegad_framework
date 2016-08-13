#ifndef __IEGAD_TCP_EVENT__
#define __IEGAD_TCP_EVENT__


#include <boost/function.hpp>
#include <boost/smart_ptr.hpp>


namespace iegad {
namespace net {


class tcp_session;


class tcp_event {
public:
    typedef boost::shared_ptr<tcp_event> ptr_t;
    typedef boost::shared_ptr<tcp_session> tcp_session_ptr;

    virtual void on_connected(tcp_session_ptr session) = 0;
    virtual void on_closed(tcp_session_ptr session) = 0;
    virtual void on_readed(tcp_session_ptr session) = 0;
    virtual void on_writed(tcp_session_ptr session) = 0;
}; // class tcp_event;


} // namespace net;
} // namespace iegad;



#endif // __IEGAD_TCP_EVENT__
