#include <thread>
#include <ctime>
#include <iostream>

#include "thrift_ex/gen-cpp/EchoServer.h"
#include "thrift_ex/gen-cpp/EchoServer_server.skeleton.hpp"
#include "thrift_ex/gen-cpp/echo_types.h"


#include "thrift_ex/thrift_server.hpp"
#include "thrift_ex/thrift_client.hpp"



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




DECLARE_EVENT_HANDLER(EchoEvent);
//DECLARE_THREADED_SERVER(EchoServer, JSON_PROTOCOL);
//DECLARE_THREADPOOL_SERVER(EchoServer, BINARY_PROTOCOL);
DECLARE_NON_BLOCKING_SERVER(EchoServer, BINARY_PROTOCOL);
DECLARE_PROXY(EchoServer, JSON_PROTOCOL);

DECLARE_CREATE_CONTEXT(myCreateContext, in, out)
{
	return nullptr;
}

DECLARE_PRE_SERVER(myPreServer)
{
	std::cout << "PreServer\n";
}




int
main(int argc, char * argv[])
{
	using ::apache::thrift::transport::TSocket;
	using ::apache::thrift::transport::TTransport;
	using ::apache::thrift::protocol::TBinaryProtocol;
	using ::apache::thrift::protocol::TProtocol;
	using ::apache::thrift::transport::TFramedTransport;

	if (argc < 2) {
		exit(1);
	}
	try {
		if (std::stoi(argv[1]) > 1000) {
			// server
			boost::shared_ptr<EchoEvent> ev(new EchoEvent);
			ev->OnPreServeEvent = myPreServer;
			ev->OnCreateContextEvent = myCreateContext;
			iegad::thrift_ex::THost host(6688, ev, 1);
			
			host.Run();
		}
		else {
			// client
			std::clock_t start, finish;

			iegad::thrift_ex::TProxy proxy("127.0.0.1", 6688);
			std::string res;
			start = std::clock();
			for (int i = 0, n = std::stoi(argv[1]); i < n; i++) {
				std::cin.get();
				proxy.imp()->echo(res, sendMsgStr);
				std::cout << res << std::endl;
			}
			finish = std::clock();
			std::cout << argv[1]
				<< "s message and  with per message's size : "
				<< sendMsgStr.size()
				<< "\ntimespan : "
				<< ((double)finish - start) / CLOCKS_PER_SEC
				<< " seconds!" << std::endl;
		}
	}
	catch (std::exception & ex) {
		std::cout << ex.what();
	}
	exit(0);
}