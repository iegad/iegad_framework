#include<iostream>
#include "iegad/net/iegad_server.hpp"


int
main(int argc, char* argv[]) {
	assert(!iegad::net::init());

	iegad::net::server::ptr_t server = iegad::net::server::create();
	server->run("0.0.0.0", "9988");
}