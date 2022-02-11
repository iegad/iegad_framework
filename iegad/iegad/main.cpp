#include<iostream>
#include "iegad/net/iegad_server.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>


class EchoProc: public iegad::net::IProcessor {
public:
	void
	onMessage(const std::string &data) override
	{
		
	}

	void
	onConnected(evutil_socket_t fd) override
	{
		spdlog::info("{} has connected\n", fd);
	}

	void
	onDisconnected(evutil_socket_t fd) override
	{
		spdlog::info("{} has disconnected\n", fd);
	}
};


int
main(int argc, char *argv[]) {
	assert(!iegad::net::init());

	spdlog::cfg::load_env_levels();
	spdlog::info("1111....");

	EchoProc* proc = new EchoProc();
	iegad::net::server::ptr_t server = iegad::net::server::create(proc);
	server->run("0.0.0.0", "9900");
}