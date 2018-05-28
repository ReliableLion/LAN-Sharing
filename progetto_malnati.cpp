
#include "stdafx.h"

#include <iostream>
#include "Server.hpp"

#pragma comment(lib,"ws2_32.lib") //Required for WinSock

int main()
{

	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		std::cout << "WinSock startup fail" << std::endl;
		exit(1);
	}

	Server server(1500);
	server.start_server();
	while (1);

	return 0;
}