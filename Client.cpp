#include "UDPService.hpp"
#include "Discovery.hpp"
#include "Session.hpp"
#include <future>
#include <iostream>     //for using cout
#include <stdlib.h>     //for using the function sleep

using namespace std;


void start_server() {

	udp_service::udp_server server;

	cout << "Started server" << endl;

	char buffer[MAXBUFL] = "";
	char addr_string_[INET_ADDRSTRLEN];
	struct sockaddr_in server_address, client_address, *client_address_ptr;

	client_address_ptr = &client_address;
	memset(&client_address, 0, sizeof(client_address));

	const auto address_len = server.receive_datagram(buffer, client_address_ptr, sizeof(buffer));

	inet_ntop(AF_INET, &(client_address_ptr->sin_addr), addr_string_, INET_ADDRSTRLEN);

	cout << "Here the message: " << buffer << "from: " << addr_string_ << endl;

	server.send_datagram(buffer, &client_address, address_len, strlen(buffer));
}

int main(int argc, char* argv[]){

	try {

		//Winsock Startup
		WSAData wsaData;
		WORD DllVersion = MAKEWORD(2, 1);
		if (WSAStartup(DllVersion, &wsaData) != 0) {
			MessageBoxA(0, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
			exit(0);
		}

		auto f1 = std::async(start_server);
		Sleep(2000);         //make the programme waiting for 5 seconds
		cout << "Started client" << endl;

		udp_service::udp_client client;

		client.get_server_info("192.168.1.102", std::to_string(UDP_PORT));

		client.send_datagram("prova");
		cout << "Datagram sent" << endl;

		client.receive_datagram();

		/*
		session::TCPConnection connection;
		cout << "Trying to connect" << endl;
		if(connection.connect_to("192.168.1.14", DEFAULT_LISTEN_PORT)) {

			cout << "Connected!" << endl;

			FILETIME filetime;
			filetime.dwLowDateTime = 1;
			filetime.dwHighDateTime = 0;

			RequestMessage message(1, filetime,"prova");
			message.prepare_message();

			request_struct structure = message.get_request_data();

			cout << "Message prepared! Trying to send it..." << endl;
			connection.send_all(reinterpret_cast<const char*>(message.get_packet_data().data()), message.get_packet_data().size());
			cout << "Message sent!" << endl;

			connection.close_connection();

		} else
			cout << "Connection error" << endl;

		*/

		return 0;
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}