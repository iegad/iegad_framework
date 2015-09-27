#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include "msg_basic.pb.h"
#include "echo_msg.pb.h"



int
main(int argc, char * argv[])
{
    WSADATA wd;
    int sockfd, n;
    sockaddr_in saddr;
    iegad::net::tcp_msg msg;
    std::string msgstr;

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6688);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    WSAStartup(0x0202, &wd);

    for (int i = 0; i < 1; i++) {
	if (sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP),
	    sockfd == -1) {
	    std::cout << WSAGetLastError() << std::endl;
	    goto exit_case;
	}

	if (connect(sockfd, (sockaddr *)&saddr, sizeof(sockaddr)) == -1) {
	    std::cout << WSAGetLastError() << std::endl;
	    goto exit_case;
	}


	std::string echo_msg_str;

	iegad::net::echo_msg echo_msg_;
	echo_msg_.set_resp_str("Hello world : res");
	echo_msg_.set_requ_str("Hello world : req");
	echo_msg_.SerializeToString(&echo_msg_str);

	msg.set_msg_type(10);
	msg.set_msg_flag(10);
	msg.set_msg_bdstr(echo_msg_str);


	msg.SerializeToString(&msgstr);
	n = send(sockfd, msgstr.c_str(), msgstr.size(), 0);
	shutdown(sockfd, SD_SEND);
	char recvbuf[1024];
	n = recv(sockfd, recvbuf, 1024, 0);
	recvbuf[n] = 0;
	std::cout << recvbuf << std::endl;

	closesocket(sockfd);
    }



exit_case:
    std::cin.get();
    exit(0);
}