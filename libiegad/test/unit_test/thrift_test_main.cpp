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



DEFINE_EVENT_HANDLER(EchoEvent);
DEFINE_NON_BLOCKING_SERVER(EchoServer, BINARY_PROTOCOL);
DEFINE_NON_BLOCKING_CLIENT(EchoClient, EchoServer, BINARY_PROTOCOL);



DEFINE_CREATE_CONTEXT(OnCreateContext, in, out)
{
	iINFO << "create context\n";
	return nullptr;
}

DEFINE_PRE_SERVER(OnPreServer)
{
	iINFO << "PreServer\n";
}


DEFINE_DELETE_CONTEXT(OnDeleteContext, txt, in, out)
{
	iINFO << "delete\n";
}


DEFINE_PROCESS_CONTEXT(OnProcessContext, txt, trans)
{
	iINFO << trans->getOrigin() << std::endl;
}



int
main(int argc, char * argv[])
{
	using ::apache::thrift::transport::TSocket;
	using ::apache::thrift::transport::TTransport;
	using ::apache::thrift::protocol::TBinaryProtocol;
	using ::apache::thrift::protocol::TProtocol;
	using ::apache::thrift::transport::TFramedTransport;
	//iegad::tools::_LOG svcLog(argv[0]);
	if (argc < 2) {
		exit(1);
	}
	try {
		if (std::stoi(argv[1]) > 1000) {
			// server
			boost::shared_ptr<iegad::thrift_ex::EchoEvent> ev(new iegad::thrift_ex::EchoEvent);
			ev->PreServeEvent = OnPreServer;
			ev->CreateContextEvent = OnCreateContext;
			ev->DeleteContextEvent = OnDeleteContext;
			ev->ProcessContextEvent = OnProcessContext;
			iegad::thrift_ex::EchoServer host(6688, ev, 1);
			host.Run();
		}
		else {
			// client
			std::clock_t start, finish;

			iegad::thrift_ex::EchoClient proxy("127.0.0.1", 6688);
			std::string res;
			start = std::clock();
			for (int i = 0, n = std::stoi(argv[1]); i < n; i++) {
				//std::cin.get();
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
		iINFO << ex.what();
	}
	exit(0);
}