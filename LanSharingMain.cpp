//#include "UDPService.hpp"
//#include "Discovery.hpp"
//#include <future>
#include <iostream>     //for using cout
#include <stdlib.h>     //for using the function sleep
#include "Server.hpp"
#include "RequestHandler.hpp"
#include "Exceptions.hpp"
#include "Discovery.hpp"

using namespace std;

void test1();
void testServer();
void test2();
void start_server();
void test3();


int main(int argc, char* argv[]) {

	//Winsock Startup
	WSAData wsa_data;
	const WORD dll_version = MAKEWORD(2, 1);
	if (WSAStartup(dll_version, &wsa_data) != 0) {
		MessageBoxA(0, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}
	testServer();
	return 0;
}

void test1() {
	try {
		//upload_manager upload;
		//request_handler request(make_shared<upload_manager>(upload));

		cout << "Ora creo la richiesta e quindi leggo il file" << endl;

		//request.send_request("192.168.1.1", "D:\\Documenti\\test\\test.txt");

		cout << "Richeista creata e file letto" << endl;

		Sleep(10000);
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

void testServer() {

	try {
		Server server;
		server.start_server();
		while (1);
//		this_thread::sleep_for(std::chrono::milliseconds(5000));
//		std::cout << "ciao!" << std::endl;
//		server.close_server();

	}
	catch (SocketException& se) {
		UNREFERENCED_PARAMETER(se);
		std::cout << "the server returned an exception" << std::endl;
	}

}

void test3() {
	UploadManager upload;

	RequestHandler req(std::make_shared<UploadManager>(upload));
	req.send_request(std::string("192.168.1.102"), "C:\\Users\\utente pc\\test_project\\10MB.zip");
}

void test2() {
	/*
	Discovery discovery_service = Discovery("davide");
	cout << "Starting discovery service" << endl;
	discovery_service.start_discovery_service();
	cout << "Discovery service started" << endl;
	Sleep(3000);
	if (discovery_service.get_online_users().size() == 0)
	cout << "NO USERS" << endl;
	else
	for (auto user : discovery_service.get_online_users()) {
	cout << "USER: " << user.first << ":" << user.second << endl;
	}
	cout << "Sending discovery..." << endl;
	discovery_service.find_users();
	cout << "Discovery sent!" << endl;
	Sleep(3000);
	if (discovery_service.get_online_users().size() == 0)
	cout << "NO USERS" << endl;
	else
	for (auto user : discovery_service.get_online_users()) {
	cout << "USER: " << user.first << ":" << user.second << endl;
	}
	*/

	//auto f1 = std::async(start_server);

	/*	Sleep(2000);         //make the programme waiting for 5 seconds
	cout << "Started client" << endl;

	udp_service::UdpClient client;

	client.get_server_info("192.168.1.102", std::to_string(UDP_PORT));

	//client.send_datagram("prova");
	client.send_broadcast(DISCOVERY_MSG);
	cout << "Datagram sent" << endl;*/

	//client.receive_datagram();

	/*	connection::TcpConnection connection("192.168.1.14", DEFAULT_LISTEN_PORT);
	cout << "Trying to connect" << endl;
	cout << "Connected!" << endl;
	FILETIME filetime;
	filetime.dwLowDateTime = 1;
	filetime.dwHighDateTime = 0;
	RequestMessage message(1, filetime, "prova");
	message.prepare_message();
	request_struct structure = message.get_request_data();
	cout << "Message prepared! Trying to send it..." << endl;
	SendSocketBuffer buffer;
	std::shared_ptr<SendSocketBuffer> bufferPtr = std::make_shared<SendSocketBuffer>(buffer);
	bufferPtr->replace(reinterpret_cast<const char*>(message.get_packet_data().data()), message.get_packet_data().size());
	connection.send_data(bufferPtr);
	cout << "Message sent!" << endl;
	connection.close_connection();*/



	try {

		UploadManager upload;
		RequestHandler request(make_shared<UploadManager>(upload));

		cout << "Ora creo la richiesta e quindi leggo il file" << endl;

		request.send_request("192.168.1.14", "D:\\Documenti\\test\\test.txt");

		cout << "Richiesta creata e file letto" << endl;

		Sleep(1000);
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}


}


/*void start_server() {
udp_service::UdpServer server;
cout << "Started server" << endl;
char buffer[MAXBUFL] = "";
struct sockaddr_in server_address, client_address;
const auto client_address_ptr = &client_address;
memset(&client_address, 0, sizeof(client_address));
//server.start_discovery_listening();
const auto address_len = server.receive_datagram(buffer, client_address_ptr, sizeof(buffer));
cout << "Here the message: " << buffer << " from: " << udp_service::get_client_address(client_address_ptr) << endl;
server.send_datagram(buffer, &client_address, address_len, strlen(buffer));
}*/