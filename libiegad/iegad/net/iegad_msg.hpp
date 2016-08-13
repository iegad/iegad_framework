#ifndef __IEGAD_MSG__
#define __IEGAD_MSG__



#include <string>
#include <boost/smart_ptr.hpp>




namespace iegad {
namespace net {




#pragma pack(1)
struct msg_head {
    uint8_t msg_flag;
    uint8_t msg_ccode;
    uint16_t msg_size;
    uint16_t msg_type;
    uint16_t msg_seq;
};
#pragma pack()


class tcp_session;



class msg_t : public boost::noncopyable {
public:
    typedef boost::shared_ptr<msg_t> ptr_t;

    static ptr_t Create() {
        ptr_t p(new msg_t);
        return p;
    }


    static ptr_t Create(boost::shared_ptr<tcp_session> session) {
        ptr_t p(new msg_t(session));
        return p;
    }

    explicit msg_t(boost::shared_ptr<tcp_session> ss)
        :
          m_tcpss(ss)
    {}

    msg_t() {}

    msg_head m_head;
    std::string m_data;
    boost::shared_ptr<tcp_session> m_tcpss;
}; // class msg_t;


} // namespace net;
} // namespace iegad;







#endif // __IEGAD_MSG__
