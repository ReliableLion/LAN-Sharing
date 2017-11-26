#include "Discovery.hpp"
#include "Session.hpp"

using namespace std;

int main(int argc, char* argv[]){

	try {

		//Winsock Startup
		WSAData wsaData;
		WORD DllVersion = MAKEWORD(2, 1);
		if (WSAStartup(DllVersion, &wsaData) != 0) {
			MessageBoxA(0, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
			exit(0);
		}

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

		return 0;
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}