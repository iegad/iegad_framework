#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libprotoc.lib")
#pragma comment(lib, "libprotobuf.lib")

#include <iostream>
#include <WinSock2.h>
#include <codecvt>
#include "msg_basic.pb.h"
#include "echo_msg.pb.h"
#include <locale>




void 
Init_Environment()
{
    WSADATA wd;
    sockaddr_in saddr;


    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6688);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    WSAStartup(0x0202, &wd);
}

int 
get_sockfd() {
    int sockfd;
    sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6688);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

BEGIN_CASE:

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sockfd > 0) {
	if (connect(sockfd, (sockaddr *)&saddr, sizeof(sockaddr)) == -1) {
	    std::cout << WSAGetLastError() << std::endl;
	    closesocket(sockfd);
	    goto BEGIN_CASE;
	}
	return sockfd;
    }
    goto BEGIN_CASE;
}



int 
send_msg(int sockfd, int msg_type, const std::string & msg_bdstr)
{
    iegad::net::msg_basic msg;
    std::string msgstr;
    int n, nleft;
    const char * p;

    msg.set_msg_type(msg_type);
    msg.set_msg_flag(10);
    msg.set_msg_bdstr(msg_bdstr);
    msg.SerializeToString(&msgstr);

    p = msgstr.c_str();
    nleft = msgstr.size();
    do {
	n = send(sockfd, p, nleft, 0);
	if (n <= 0) {
	    break;
	}
	nleft -= n;
	p += n;
    } while (n > 0);
    shutdown(sockfd, SD_SEND);
    return msgstr.size() - nleft;
}



const std::string 
echo_svc_proc(const std::string & echo_str)
{
    int sockfd;
    sockfd = get_sockfd();
    iegad::net::echo_msg echo_msg_;
    std::string echo_msg_str;
    char buffer[1024];
    int n;
    echo_msg_.set_requ_str(echo_str.c_str());
    if (echo_msg_.SerializeToString(&echo_msg_str)) {
	send_msg(sockfd, 10, echo_msg_str);
    }
    
    n = recv(sockfd, buffer, 1024, 0);
    buffer[n] = 0;
    closesocket(sockfd);
    return buffer;
}


enum {
    N_TIMES = 100
};


int
main(int argc, char * argv[])
{
    int nCount = 0;
    Init_Environment();
    std::locale::global(std::locale("chs"));

    for (int i = 0; i < N_TIMES; i++) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	// 当发送长度为127时, 发送无警告, 否则, protocol buffer会发出警告
	std::string resp = conv.to_bytes(L"肖琪是超级大天才 肖琪是超级大天才 肖琪是超级大天才 肖琪是超级大天才 肖琪是超级大天才!!!");
	std::string rzt;
	rzt = echo_svc_proc(resp);
	std::wcout << conv.from_bytes(rzt) << std::endl;
	nCount++;
    }
    std::cout << nCount << " times done" << std::endl;
exit_case:
    std::cin.get();
    WSACleanup();
    exit(0);
}