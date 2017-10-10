#include "Discovery.hpp"

using namespace std;

int main(int argc, char* argv[]){
	try
	{

		Discovery service = Discovery("prova");

		cout << "Starting UDP server..." << endl;

		service.startDiscoveryService();

		cout << "UDP server started!" << endl << "Sending boradcast message..." << endl;
		
		/*auto temp = service.findUser("192.168.1.102", "1234");

		cout << "Broadcast sent!" << endl;

		auto iterator = temp.begin();

		cout << "This is the packet: " << (*iterator).first << endl;*/

		/*	if (argc != 2)
		{
		std::cerr << "Usage: ping <host>" << std::endl;
		#if !defined(BOOST_WINDOWS)
		std::cerr << "(You may need to run this program as root.)" << std::endl;
		#endif
		return 1;
		}*/

		//boost::asio::io_service io_service;
		//Pinger p(io_service, argv[1], 3);
		//io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}