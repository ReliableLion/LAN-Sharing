#pragma once
#include <WinSock2.h>

namespace session {

	class TCPConnection {

	private:
		bool connection_state;
		SOCKET sock;

	};

	class ProtocolManager {

	};

	class Endpoint {

	};

	class SessionManager {

		Endpoint remote_endpoint;
		TCPConnection connection;

	};

}