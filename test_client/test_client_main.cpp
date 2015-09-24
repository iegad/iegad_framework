#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include "msg_basic.pb.h"



int
main(int argc, char * argv[])
{
    WSADATA wd;
    int sockfd, n;
    sockaddr_in saddr;
    char buffer[] = "www.iegad.com";
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

	//msg.set_msg_type(10);
	//msg.set_msg_flag(1);
	//msg.set_msg_bdstr(buffer);

	//msg.SerializeToString(&msgstr);
	//n = send(sockfd, msgstr.c_str(), msgstr.size(), 0);
	for (int j = 0; j < 100; j++) {
	    n = send(sockfd, buffer, sizeof(buffer) - 1, 0);
	    if (n <= 0) {
		std::cout << "miss connection...\n";
		break;
	    }
	}

	closesocket(sockfd);
    }



exit_case:
    std::cin.get();
    exit(0);
}