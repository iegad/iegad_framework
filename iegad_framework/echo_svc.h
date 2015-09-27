#ifndef __ECHO_SVC__
#define __ECHO_SVC__


#include "service_basic.h"
#include "Msg/echo_msg.pb.h"
#include "iegad_log.hpp"


namespace iegad {
namespace net {


    class echo_svc : public iegad::net::svc_basic {
  
    public:
	explicit echo_svc(int svc_id) : iegad::net::svc_basic(svc_id) {}

	virtual int action(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr)
	{
	    if (!msgdata_.ParseFromString(msg_bdstr)) {
		iERR << "echo_msg build failed" << std::endl;
		return -1;
	    }
	    clnt.write_some(boost::asio::buffer(msgdata_.resp_str()));
	    return 0;
	}

    private:
	iegad::net::echo_msg msgdata_;
    };


} // namespace net;
} // namespace iegad;


#endif // __ECHO_SVC__