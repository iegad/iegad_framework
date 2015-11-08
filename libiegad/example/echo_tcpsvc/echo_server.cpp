#include "nets/tcp_mt_svc.h"
#include "nets/basic_svc.h"
#include "iegad_define.h"


#include <iostream>


#pragma comment(lib, "libiegad_nets.lib")
#pragma comment(lib, "libiegad_common.lib")
#pragma comment(lib, "libiegad_msg.lib")



class echo_svc : public iegad::nets::basic_svc {
public:
    enum {
	ECHO_SVC = 10
    };
    explicit echo_svc() 
	: iegad::nets::basic_svc(ECHO_SVC) {}


    virtual int 
	invoke(boost::asio::ip::tcp::socket & clnt, int msg_flag, const std::string & msg_bdstr) {
	boost::system::error_code err_code;
	this->_return(clnt, msg_bdstr.c_str(), msg_bdstr.size(), err_code);
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
    host.regist_svc(echoSvc);
    host.run(1);
    std::cin.get();
    exit(0);
}


