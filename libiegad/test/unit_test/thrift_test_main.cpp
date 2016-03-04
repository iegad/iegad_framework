#include <thread>
#include <ctime>
#include <iostream>
#include "thrift_ex/Astroboy.svr.hpp"
#include "thrift_ex/gen-cpp/Astroboy_server.skeleton.hpp"



std::string sendMsgStr = "君不见黄河之水天上来⑵，奔流到海不复回。\n"
"君不见高堂明镜悲白发，朝如青丝暮成雪⑶。\n"
"人生得意须尽欢⑷，莫使金樽空对月。\n"
"天生我材必有用，千金散尽还复来。\n"
"烹羊宰牛且为乐，会须一饮三百杯⑸。\n"
"岑夫子，丹丘生⑹，将进酒，杯莫停⑺。\n"
"与君歌一曲⑻，请君为我倾耳听。\n"
"钟鼓馔玉不足贵⑽，但愿长醉不复醒⒄。\n"
"古来圣贤皆寂寞，惟有饮者留其名。\n"
"陈王昔时宴平乐，斗酒十千恣欢谑⑿。\n"
"主人何为言少钱⒀，径须沽取对君酌⒁。\n"
"五花马⒂，千金裘，呼儿将出换美酒，与尔同销万古愁⒃。\n";


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

    if (argc < 2) {
	std::cout << "参数错误\n";
	exit(1);
    }

    try {
	if (argc > 1 && std::stoi(argv[1]) < 1000) {
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
		Astroboy_svr astro(6688, 4);
		astro.Run();
	    } while (false);
	}
	else {
	    // client
	    std::clock_t start, finish;
	    boost::shared_ptr<TSocket> sock(new TSocket("127.0.0.1", 6688));
	    boost::shared_ptr<TBinaryProtocol> proto(new TBinaryProtocol(sock));
	    boost::shared_ptr<AstroboyClient> client(new AstroboyClient(proto));

	    start = std::clock();

	    for (int i = 0, n = std::stoi(argv[1]); i < n; i++) {
		res_t res;
		req_t req;

		req.__set_r_type(1);
		req.__set_r_str(sendMsgStr);
		sock->open();
		client->Elefun(res, req);
		if (!res.__isset.r_str) {
		    std::cout << "failed" << std::endl;
		}
	    }
	    sock->close();

	    finish = std::clock();
	    std::cout << argv[1] << "s message and  with per message's size : "<< sendMsgStr.size() << "\ntimespan : "<< ((double)finish - start) / CLOCKS_PER_SEC << " seconds!" <<std::endl;
	}
    }
    catch (std::exception & ex) {
	std::cout << ex.what() << std::endl;
    }
    exit(0);
}