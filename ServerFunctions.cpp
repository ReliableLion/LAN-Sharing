#include "pch.h"

// definition of the function exposed by the dll
extern "C" __declspec(dllexport) BOOL start_lan_sharing(std::string path);
extern "C" __declspec(dllexport) VOID pause_server();
extern "C" __declspec(dllexport) VOID recover_server();
extern "C" __declspec(dllexport) VOID change_server_dw_path(std::string new_path);

Server main_server;

BOOL start_lan_sharing(std::string path) {
	try {

		//Winsock Startup
		WSAData wsa_data;
		const WORD dll_version = MAKEWORD(2, 1);
		if (WSAStartup(dll_version, &wsa_data) != 0) {
			MessageBoxA(0, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
			managed_callback::exit_callback();
		}

		try {
			main_server.start_server(1500, path);
		}
		catch (std::exception &e) {
			std::cout << "Exception thrown by main server: " << WSAGetLastError() << " " << GetLastError() << std::endl;
			return FALSE;
		}

		return TRUE;

	}
	catch (SocketException& se) {
		UNREFERENCED_PARAMETER(se);
		std::cout << "server returned an exception" << std::endl;

		return FALSE;
	}
	catch (ServerException &se) {
		UNREFERENCED_PARAMETER(se);
		std::cout << "Impossible to start server because is already running" << std::endl;

		return FALSE;
	}
}

VOID pause_server() {
	main_server.pause_server();
}

VOID recover_server() {
	main_server.recover_server();
}

VOID change_server_dw_path(std::string new_path) {
	main_server.change_dest_path(new_path);
}
