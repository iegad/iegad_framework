#include <thread>
#include <ctime>
#include <iostream>

#include "thrift_ex/gen-cpp/EchoServer.h"
#include "thrift_ex/gen-cpp/EchoServer_server.skeleton.hpp"
#include "thrift_ex/gen-cpp/echo_types.h"


#include "thrift_ex/thrift_server.hpp"
#include "thrift_ex/thrift_client.hpp"



std::string sendMsgStr = "�������ƺ�֮ˮ�������ƣ��������������ء�\n"
"�����������������׷���������˿ĺ��ѩ�ǡ�\n"
"���������뾡���ȣ�Īʹ���׿ն��¡�\n"
"�����Ҳı����ã�ǧ��ɢ����������\n"
"������ţ��Ϊ�֣�����һ�����ٱ��ɡ�\n"
"᯷��ӣ��������ʣ������ƣ���Īͣ�ˡ�\n"
"�����һ���̣����Ϊ���������\n"
"�ӹ��������Σ���Ը�������Ѣա�\n"
"����ʥ�ͽԼ�į��Ω��������������\n"
"������ʱ��ƽ�֣�����ʮǧ���ʢС�\n"
"���˺�Ϊ����Ǯ�ѣ������ȡ�Ծ��âҡ�\n"
"�廨��ӣ�ǧ���ã��������������ƣ����ͬ����ų�ԡ�\n";




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