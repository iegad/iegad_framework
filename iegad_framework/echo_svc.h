#ifndef __ECHO_SVC__
#define __ECHO_SVC__


#include "service_basic.h"
#include "Msg/echo_msg.pb.h"
#include "iegad_log.h"


namespace iegad {
namespace net {


    class echo_svc : public iegad::net::svc_basic {
  
    public:
	explicit echo_svc(int svc_id) : 
	    iegad::net::svc_basic(svc_id) 
	{}

	virtual int action(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr)
	{
	    if (this->_build_svc<iegad::net::echo_msg>(msg_bdstr, msgdata_) != 0) {
		iERR << "echo_msg build failed" << std::endl;
		return -1;
	    }
	    if (this->_response(clnt, msgdata_.requ_str().c_str(), 
		msgdata_.requ_str().size()) != msgdata_.requ_str().size()) {
		iERR << "echo_msg responce failed" << std::endl;
	    }
	    return 0;
	}

    private:
	iegad::net::echo_msg msgdata_;
    }; // class echo_svc;


} // namespace net;
} // namespace iegad;


#endif // __ECHO_SVC__