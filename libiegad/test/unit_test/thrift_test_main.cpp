#include <thread>
#include <ctime>
#include <iostream>
#include "common/Astroboy.svr.hpp"


bool InitEnvironment() {
    return true;
}


class EchoImp : public iegad::thrift_ex::IElefunImp {
public:
    EchoImp(int32_t funno)
	: iegad::thrift_ex::IElefunImp(funno) {}


    virtual int32_t _action(const std::string & msgstr, int32_t msgsize,
				    std::string * resstr, int32_t * ressize,
				    int32_t * errcode, std::string * errstr) override 
    {
	*resstr = msgstr;
	*errstr = "1234567899";
	return 0;
    }
}; // class EchoImp;


int
main(int argc, char * argv[])
{
    using ::apache::thrift::transport::TSocket;
    using ::apache::thrift::transport::TTransport;
    using ::apache::thrift::protocol::TBinaryProtocol;
    using ::apache::thrift::protocol::TProtocol;
    using iegad::thrift_ex::AstroboyClient;
    using iegad::thrift_ex::req_t;
    using iegad::thrift_ex::res_t;

    if (std::stoi(argv[1]) > 0) {
    // server
	using iegad::thrift_ex::Astroboy_svr;
	using iegad::thrift_ex::AstroboyHandler;
	do {
	    if (!Astroboy_svr::InitEnvironment(InitEnvironment)) {
		break;
	    }
	    std::shared_ptr<Astroboy_svr::action_map_t> map(new Astroboy_svr::action_map_t);
	    std::shared_ptr<EchoImp> echoImp(new EchoImp(1));
	    map->insert(std::make_pair(echoImp->No(), echoImp));
	    AstroboyHandler::SetActionMap(map);
	    Astroboy_svr astro(6688);
	    astro.Run();
	} while (false);
    }
    else {
    // client
	boost::shared_ptr<TSocket> sock(new TSocket("127.0.0.1", 6688));
	boost::shared_ptr<TBinaryProtocol> proto(new TBinaryProtocol(sock));
	boost::shared_ptr<AstroboyClient> client(new AstroboyClient(proto));

	res_t res;
	req_t req;
	req.__set_r_type(1);
	req.__set_r_str("Ð¤ç÷ÊÇÌì²Å");
	sock->open();
	client->Elefun(res, req);
	if (res.__isset.r_str && res.__isset.r_errstr) {
	    std::cout << res.r_str << res.r_errstr << std::endl;
	}	
	sock->close();
    }

    std::cin.get();
    exit(0);
}