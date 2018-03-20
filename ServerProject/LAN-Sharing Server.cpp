#include "Server.hpp"

#include <iostream>
#include <signal.h>

#pragma comment(lib, "Ws2_32.lib")

void sigBreak_handler(int n_signal);

int main(int argc, char* argv[]) {

	Server server;
	std::cout << "new server instance created" << std::endl;
	signal(SIGBREAK, sigBreak_handler);

	while (true)
	{
		std::cout << "waiting for incoming request..." << std::endl;
		//server.listenNewConnection();						// listen for new connections
		std::cout << std::endl;
	}

	return 1;
}

void sigBreak_handler(int n_signal)
{
	std::cout << "user terminate this process" << std::endl;
	exit(1);
}