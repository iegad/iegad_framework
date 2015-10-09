#ifndef __ECHO_SVC__
#define __ECHO_SVC__


#include "svc/basic_svc.h"
#include "msg/echo_msg.pb.h"
#include "common/iegad_log.h"


namespace iegad {
namespace net {


    class echo_svc : public iegad::net::basic_svc {
  
    public:
	explicit echo_svc(int svc_id);

	virtual int action(boost::asio::ip::tcp::socket & clnt, 
	    int msg_flag, const std::string & msg_bdstr);

    private:

    }; // class echo_svc;


    typedef std::shared_ptr<echo_svc> echo_svc_ptr;


} // namespace net;
} // namespace iegad;


#endif // __ECHO_SVC__