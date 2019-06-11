#include "pch.h"

using namespace connection;

/*
* this constructor create a new socket that is setted on the defaul port 1500;
* if you want to change it is possible to call the method change port and pass the new port number, t
*/
Server::Server(): server_status_(stopped) {}

Server::~Server() {
    close_server();
}

void Server::run_server() {
    std::cout << "Server is running" << std::endl << std::endl;

    while (server_status_.load() == running) {
        listen_new_connection();
    }
}
inline std::tm localtime_xp(std::time_t timer)
{
    std::tm bt {};
#if defined(__unix__)
    localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
    localtime_s(&bt, &timer);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    bt = *std::localtime(&timer);
#endif
    return bt;
}

// default = "YYYY-MM-DD HH:MM:SS"
inline std::string time_stamp(time_t time_stamp_t, const std::string& fmt = "%F %T")
{
    auto bt = localtime_xp(time_stamp_t);
    char buf[64];
    return {buf, std::strftime(buf, sizeof(buf), fmt.c_str(), &bt)};
}
void Server::print_client_info(const std::chrono::time_point<std::chrono::system_clock> time_point,
                               std::shared_ptr<TcpConnection> connection) {

    auto time_stamp_t = std::chrono::system_clock::to_time_t(time_point);

    std::cout << "***************************************************" << std::endl;
    std::cout << "(" << time_stamp(time_stamp_t) << ") ";
    std::cout << "Server accepted an incoming request" << std::endl << "Client information: " << std::endl;
    connection->print_endpoint_info();
    std::cout << "***************************************************" << std::endl;
}


void Server::listen_new_connection() {
	SOCKADDR_IN client_address;
    int addrlen = sizeof(client_address);

	const auto accept_socket = accept(passive_socket_, reinterpret_cast<SOCKADDR *>(&client_address), &addrlen);

	// if the server is in pause return to the caller
	if (server_status_.load() == paused) {
		return; 
	}

	if (accept_socket == INVALID_SOCKET) {
		if (WSAGetLastError() == WSAEINTR) {
			server_status_.store(stopped);
		}
		return;
	}

	std::shared_ptr<TcpConnection> new_connection = std::make_shared<TcpConnection>(accept_socket, client_address);
	
#ifdef APP_DEBUG
    print_client_info(std::chrono::system_clock::now(), new_connection);
#endif

	QueueConnectionElement element;
	element.connection = new_connection;
	element.attempts = 0;

    // add the request inside the request manager
	request_status status;
	if (!handshake_agreement_manager_->add_connection(element, status)) {
		switch (status) {
		case full_queue :
			// std::cout << "impossible to add the connection, queue is full" << std::endl;
			ConcurrentStreamPrint::print_data(class_name, "impossible to add the connection, queue is full");
			break;
		case term_signal :
			// std::cout << "impossible to add the connection because the request manager have reveived the shutdown request" << std::endl;
			ConcurrentStreamPrint::print_data(class_name, "impossible to add the connection because the request manager have reveived the shutdown request");
			break;
		default:
			break;
		}
	}
}

void Server::start_server(const int port) {

	// change the server state and instantiate the main thread
	if (server_status_.load() == running)
		throw ServerException();

	if (port < 0 || port > 65535) {
		std::cout << "the value of the port passed as parameter is not valid" << std::endl;
		exit(1);
	}

	// create the data structure that contain the local address and the server port
	local_address_.sin_addr.s_addr = htonl(INADDR_ANY);
	local_address_.sin_port = htons(static_cast<uint16_t>(port));
	local_address_.sin_family = AF_INET;

	passive_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (::bind(passive_socket_, reinterpret_cast<SOCKADDR *>(&local_address_), sizeof(local_address_)) == SOCKET_ERROR) {
		std::cout << "impossible to bind the socket with the specified address, error:"
			<< std::to_string(WSAGetLastError()) << std::endl;
		exit(1);
	}

	if (listen(passive_socket_, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "server cannot listen for incoming request, error: "
			<< std::to_string(WSAGetLastError())
			<< std::endl;
		exit(1);
	}

	// convert the address from sockaddr type into a human readable format
	char address_msg[1024];
	inet_ntop(AF_INET, &(local_address_.sin_addr), address_msg, 1024);

	std::cout << "This server has address: " << address_msg;
	std::cout << ", port number: " << ntohs(local_address_.sin_port) << std::endl << std::endl;

	download_manager_ = std::make_shared<DownloadManager>();
    handshake_agreement_manager_ = std::make_shared<HandshakeManager>(download_manager_);

    server_status_.store(running);
    server_main_thread_ = std::thread(&Server::run_server, this);
}

void Server::close_server() {
    closesocket(passive_socket_);
    server_main_thread_.join();

    handshake_agreement_manager_.reset();
    download_manager_.reset();

	server_status_.store(stopped);
}

void Server::pause_server() {
	if (server_status_.load() == running) {
		server_status_.store(paused);
	}
}

void Server::recover_server() {
	if (server_status_.load() == paused) {
		server_status_.store(running);
	}
}
