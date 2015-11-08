#include "nets/tcp_mt_svc.h"
#include "nets/basic_svc.h"
#include "iegad_define.h"


#include <iostream>
#include "msg/basic_msg.pb.h"


#ifdef WIN32

#pragma comment(lib, "libiegad_nets.lib")
#pragma comment(lib, "libiegad_common.lib")
#pragma comment(lib, "libiegad_msg.lib")
#pragma comment(lib, "libglog.lib")
#pragma comment(lib, "libprotobuf.lib")

#endif // WIN32




class echo_svc : public iegad::nets::basic_svc {
public:
    enum {
	ECHO_SVC = 10
    };
    echo_svc() 
	: iegad::nets::basic_svc(ECHO_SVC) {}


    virtual int 
	invoke(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr) override {
	boost::system::error_code err_code;
	this->_return(clnt, msg_bdstr.c_str(), msg_bdstr.size(), err_code);
	if (err_code) {
	    std::cout << err_code.message() << std::endl;
	}
	return 0;
    }
};


class echoEx_svc : public iegad::nets::basic_svc {
public:
    enum {
	ECHOEX_SVC = 11
    };
    echoEx_svc()
	: iegad::nets::basic_svc(ECHOEX_SVC) {}


    virtual int
    invoke(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr) override {
	boost::system::error_code err_code;
	this->_send_msg(clnt, msg_flag, msg_bdstr, err_code);
	if (err_code) {
	    std::cout << err_code.message() << std::endl;
	}
	return 0;
    }
};



int
main(int argc, char * argv[])
{
    iegad::nets::tcp_mt_svc host("127.0.0.1", "6688");
    std::shared_ptr<echo_svc> echoSvc(new echo_svc());
    std::shared_ptr<echoEx_svc> echoExSvc(new echoEx_svc());
    host.regist_svc(echoSvc);
    host.regist_svc(echoExSvc);
    host.run(4);
    std::cin.get();
    exit(0);
}


