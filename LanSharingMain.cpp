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
	//test3();
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
	} catch (SocketException& se) {
		UNREFERENCED_PARAMETER(se);
		std::cout << "server returned an exception" << std::endl;
	}
	catch (ServerException &se) {
		UNREFERENCED_PARAMETER(se);
		std::cout << "Impossible to start server because is alreasy runnig" << std::endl;
	}
}

void test3() {
	RequestHandler req;
	user alessandro = user("Alessandro Ianne", "192.168.1.102");

	auto request1 = user_request(alessandro,"C:\\Users\\utente pc\\test_project\\100MB.zip");
	auto request2 = user_request(alessandro,"C:\\Users\\utente pc\\test_project\\100MB_2.zip");
	auto request3 = user_request(alessandro,"C:\\Users\\utente pc\\test_project\\prova.txt");

	req.send_request(request1);

	req.send_request(request2);

	req.send_request(request3);

	if(!req.is_terminated(request1)) {
		cout << "Transferring of " << request1.file_name << " to " << request1.destination_user.username << " is not terminated" << endl;
	} else {
		cout << "Transferring of " << request1.file_name << " to " << request1.destination_user.username << " terminated!" << endl;
	}

	if(!req.is_terminated(request2)) {
		cout << "Transferring of " << request2.file_name << " to " << request2.destination_user.username << " is not terminated" << endl;
	} else {
		cout << "Transferring of " << request2.file_name << " to " << request2.destination_user.username << " terminated!" << endl;
	}

	if(!req.is_terminated(request3)) {
		cout << "Transferring of " << request3.file_name << " to " << request3.destination_user.username << " is not terminated" << endl;
	} else {
		cout << "Transferring of " << request3.file_name << " to " << request3.destination_user.username << " terminated!" << endl;
	}


	Sleep(10000);

	if(!req.is_terminated(request1)) {
		cout << "Transferring of " << request1.file_name << " to " << request1.destination_user.username << " is not terminated" << endl;
	} else {
		cout << "Transferring of " << request1.file_name << " to " << request1.destination_user.username << " terminated!" << endl;

		if(req.get_result(request1))
			cout << "Transferred correctly" << endl;
		else
			cout << "There was some problems" << endl;
	}

	if(!req.is_terminated(request2)) {
		cout << "Transferring of " << request2.file_name << " to " << request2.destination_user.username << " is not terminated" << endl;
	} else {
		cout << "Transferring of " << request2.file_name << " to " << request2.destination_user.username << " terminated!" << endl;

		if(req.get_result(request2))
			cout << "Transferred correctly" << endl;
		else
			cout << "There was some problems" << endl;
	}

	if(!req.is_terminated(request3)) {
		cout << "Transferring of " << request3.file_name << " to " << request3.destination_user.username << " is not terminated" << endl;
	} else {
		cout << "Transferring of " << request3.file_name << " to " << request3.destination_user.username << " terminated!" << endl;

		if(req.get_result(request3))
			cout << "Transferred correctly" << endl;
		else
			cout << "There was some problems" << endl;
	}

	/*auto requests_list = req.get_requests();
	for(auto it = requests_list.begin(); it != requests_list.end(); ++it)
	{
		it->second.wait();
		cout << "ORA STAMPO RISULTATO: " << it->second.get() << endl;
	}*/
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

	/*connection::TcpConnection connection("192.168.1.9", DEFAULT_LISTEN_PORT);
	cout << "Trying to connect" << endl;
	cout << "Connected!" << endl;
	FILETIME filetime;
	filetime.dwLowDateTime = 1;
	filetime.dwHighDateTime = 0;
	ProtocolMessage message(1, filetime, "prova");
	// request_struct structure = message.get_request_data();
	cout << "Message prepared! Trying to send it..." << endl;
	SendSocketBuffer buffer;
	std::shared_ptr<SendSocketBuffer> bufferPtr = std::make_shared<SendSocketBuffer>(buffer);
	bufferPtr->replace(reinterpret_cast<const char*>(message.get_packet_data().data()), message.get_packet_data().size());
	connection.send_data(bufferPtr);
	cout << "Message sent!" << endl;
	connection.close_connection();*/



	try {

		RequestHandler request;

		cout << "Ora creo la richiesta e quindi leggo il file" << endl;

		//request.send_request(user("Alessandro Ianne", "192.168.1.9"), "C:\\Users\\Asus\\Documents\\prova.txt");

		cout << "Richiesta creata e file letto" << endl;

		Sleep(1000);
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}


}


void start_server() {
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
}